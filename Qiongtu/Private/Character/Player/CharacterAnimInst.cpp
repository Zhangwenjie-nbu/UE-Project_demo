// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/CharacterAnimInst.h"

#include "AnimGraphRuntime/Public/KismetAnimationLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Core/GameCore/QTCharacterBase.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


void UCharacterAnimInst::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (!MyCharacter)
	{
		MyCharacter = Cast<AQTCharacterBase>(TryGetPawnOwner());
		if (MyCharacter)
		{
			MyCharacter->OnMovementStatesChanged.AddDynamic(this, &UCharacterAnimInst::OnMovementStatesChangedEvent);
		}
	}
	WeightGaitCurveName = "Weight_Gait";
}

void UCharacterAnimInst::UpdateAnimationProperties()
{
	if (!MyCharacter)
	{
		MyCharacter = Cast<AQTCharacterBase>(TryGetPawnOwner());
		if (MyCharacter)
		{
			MyCharacter->OnMovementStatesChanged.AddDynamic(this, &UCharacterAnimInst::OnMovementStatesChangedEvent);
		}
	}
	else
	{
		UClass* CharacterClass = MyCharacter->GetClass();
		if (CharacterClass->ImplementsInterface(UQTMovementInterface::StaticClass()))
		{
			IQTMovementInterface::Execute_SetMovementValues(MyCharacter, MovementValues);
		}
		UpdateLocalMovementValues();
	}
}

void UCharacterAnimInst::UpdateLocalMovementValues()
{
	if (MovementStates.MovementType == EQTMovementType::EGround)
	{
		UpdateMoveBlendValues();
		UpdateMovementConditionValue();
		UpdateFootIKValues();
	}
	else if (MovementStates.MovementType == EQTMovementType::EFalling)
	{
		UpdateInAirValues();
	}
	UpdateLayerValues();
}

void UCharacterAnimInst::OnMovementStatesChangedEvent(const FMovementStates& NewMovementStates)
{
	MovementStates = NewMovementStates;
	if (MovementStates.MovementType == EQTMovementType::EGround)
	{
		;
	}
	else if(MovementStates.MovementType == EQTMovementType::EFalling)
	{
		JumpPlayRate = FMath::GetMappedRangeValueClamped(FVector2d(0.0f, 600.f),
														FVector2d(1.2f, 1.5f), MovementValues.Speed); 
	}

}

float UCharacterAnimInst::CalculateStrideBlend()
{
	if (!WalkStrideCurve || !RunStrideCurve || !CrouchStrideCurve)
	{
		return 0;
	}
	float LStride = StrideBlend;
	if (MovementStates.Stance == EQTStance::ECrouch)
	{
		LStride = CrouchStrideCurve->GetFloatValue(MovementValues.Speed);
	}
	else
	{
		float GaitWeightValue = GetCurveValue(WeightGaitCurveName);
		if (GaitWeightValue <= 1.1f && GaitWeightValue > 0.8f)
		{
			LStride = WalkStrideCurve->GetFloatValue(MovementValues.Speed);
		}
		else if (GaitWeightValue > 1.1f)
		{
			LStride = RunStrideCurve->GetFloatValue(MovementValues.Speed);
		}
	}
	//由于帧数之间的时间不同机器可能不同，为了使更新速度相同，所以需要乘上时间差
	LStride = FMath::FInterpTo(StrideBlend, LStride, UGameplayStatics::GetWorldDeltaSeconds(this),
							StrideBlendInterpSpeed * UGameplayStatics::GetWorldDeltaSeconds(this));
	return FMath::Clamp(LStride, 0.0f, 1.0f);
}

float UCharacterAnimInst::CalculateWalkRunBlend()
{
	float LWalkRunBlend = WalkRunBlend;
	if (MovementStates.Gait == EQTGait::EWalk)
	{
		LWalkRunBlend -= 0.3f;
		return FMath::Clamp(LWalkRunBlend, 0.0f, 1.0f);
	}
	LWalkRunBlend += 0.1f;
	return FMath::Clamp(LWalkRunBlend, 0.0f, 1.0f);
}

float UCharacterAnimInst::CalculatePlayRate()
{
	float LPlayRate;
	float GaitWeightValue = GetCurveValue(WeightGaitCurveName);
	if (GaitWeightValue <= 1.1)
	{
		LPlayRate = MovementValues.Speed / MyCharacter->WalkSpeed;
	}
	else if (GaitWeightValue <= 2.1)
	{
		LPlayRate = MovementValues.Speed / MyCharacter->RunSpeed;
	}
	else
		LPlayRate = MovementValues.Speed / MyCharacter->SprintSpeed;
	LPlayRate = LPlayRate / (MyCharacter->GetMesh()->GetComponentScale().Z);
	return FMath::Clamp(LPlayRate, 0.0f, 3.0f);
}

float UCharacterAnimInst::CalculateCrouchingPlayRate()
{
	float LPlayRate;
	LPlayRate = MovementValues.Speed / 150.f;
	LPlayRate = LPlayRate / (MyCharacter->GetMesh()->GetComponentScale().Z) / StrideBlend;
	return FMath::Clamp(LPlayRate, 0.0f, 3.0f);
}

void UCharacterAnimInst::UpdateMoveBlendValues()
{
	RelativeAcceleration = CalculateRelativeAcceleration();
	SetVelocityBlend();
	CalculateMovementDirection();
	UpdataLeanAmount();
	WalkRunBlend = CalculateWalkRunBlend();
	StrideBlend = CalculateStrideBlend();
	PlayRate = CalculatePlayRate();
	CrouchPlayRate = CalculateCrouchingPlayRate();
}

FVector UCharacterAnimInst::CalculateRelativeAcceleration()
{
	FVector TempAcceleration;
	FRotator CharacterRotation = MyCharacter->GetActorRotation();
	//如果速度与加速度的点积大于0，说明正在向前加速
	if ((MovementValues.Velocity | MovementValues.Acceleration) > 0.0f)
	{
		//将加速度先限制角色最大加速度大小，然后将加速度向量坐标旋转到角色的当前旋转方向上，就可以得出加速度在角色旋转方向上的xyz分量。
		//然后比上角色最大加速度，得到模长为0-1的向量，即可作为混合加速冲刺与正常冲刺的混合度。
		TempAcceleration = CharacterRotation.UnrotateVector(
			MovementValues.Acceleration.GetClampedToMaxSize(MyCharacter->GetCharacterMovement()->GetMaxAcceleration())) /
				MyCharacter->GetCharacterMovement()->GetMaxAcceleration();	
	}
	else
	{	
		//同理，减速状态只是将最大加速度换为最大制动减速度
		TempAcceleration = CharacterRotation.UnrotateVector(
			MovementValues.Acceleration.GetClampedToMaxSize(MyCharacter->GetCharacterMovement()->GetMaxBrakingDeceleration())) /
				MyCharacter->GetCharacterMovement()->GetMaxBrakingDeceleration();
	}
	return TempAcceleration;
}

//设置速度各个方向的混合
void UCharacterAnimInst::SetVelocityBlend()
{
	VelocityDir = MovementValues.Velocity;
	if (!VelocityDir.Normalize(0.0001f))
	{
		return;
	}
	VelocityDir = MyCharacter->GetActorRotation().UnrotateVector(VelocityDir);
	float Sum = FMath::Abs(VelocityDir.X) + FMath::Abs(VelocityDir.Y)+ FMath::Abs(VelocityDir.Z);
	VelocityDir /= Sum;
	//通过插值让四个方向分量平滑过渡
	VelocityBlend.F = FMath::FInterpTo(VelocityBlend.F, FMath::Clamp(VelocityDir.X, 0.0f, 1.0f),
		UGameplayStatics::GetWorldDeltaSeconds(MyCharacter), VBInterpSpeed);
	VelocityBlend.B = FMath::FInterpTo(VelocityBlend.B, FMath::Abs(FMath::Clamp(VelocityDir.X, -1.0f, 0.0f)),
		UGameplayStatics::GetWorldDeltaSeconds(MyCharacter), VBInterpSpeed);
	VelocityBlend.R = FMath::FInterpTo(VelocityBlend.R, FMath::Clamp(VelocityDir.Y, 0.0f, 1.0f),
		UGameplayStatics::GetWorldDeltaSeconds(MyCharacter), VBInterpSpeed);
	VelocityBlend.L = FMath::FInterpTo(VelocityBlend.L, FMath::Abs(FMath::Clamp(VelocityDir.Y, -1.0f, 0.0f)),
		UGameplayStatics::GetWorldDeltaSeconds(MyCharacter), VBInterpSpeed);
}

void UCharacterAnimInst::CalculateMovementDirection()
{
	float Direction = UKismetAnimationLibrary::CalculateDirection(MovementValues.Velocity, MovementValues.AimingRotation);
	if (MovementStates.Gait == EQTGait::ESprint)
	{
		MoveDirection = EQTMovementDirection::EForward;
		return;
	}
	if (FMath::Abs(Direction) < FR_Threshold)
	{
		MoveDirection = EQTMovementDirection::EForward;
	}
	else if (Direction > BL_Threshold && Direction < FL_Threshold)
	{
		MoveDirection = EQTMovementDirection::ELeft;
	}
	else if (Direction > FR_Threshold && Direction < BR_Threshold)
	{
		MoveDirection = EQTMovementDirection::ERight;
	}
	else
		MoveDirection = EQTMovementDirection::EBackWard;
}

void UCharacterAnimInst::SetLeanAmount(float _FB, float _LR)
{
	GroundedLeanAmount.FB = _FB;
	GroundedLeanAmount.LR = _LR;
}

void UCharacterAnimInst::InterpLeanAmount()
{
	static float FB, LR;
	FB = FMath::FInterpTo(FB, RelativeAcceleration.X, UGameplayStatics::GetWorldDeltaSeconds(MyCharacter), GroundLeanInterpSpeed);
	LR = FMath::FInterpTo(LR, RelativeAcceleration.Y, UGameplayStatics::GetWorldDeltaSeconds(MyCharacter), GroundLeanInterpSpeed);
	SetLeanAmount(FMath::Clamp(FB, -1.0f, 1.0f), FMath::Clamp(LR, -1.0f, 1.0f));
}

void UCharacterAnimInst::UpdataLeanAmount()
{
	InterpLeanAmount();
}

void UCharacterAnimInst::UpdateMovementConditionValue()
{
	IsMoving = MyCharacter->IsMoving();
	HasMoveInput = MyCharacter->HasMovementInput();
}

void UCharacterAnimInst::UpdateFootIKValues()
{
	SetFootLocking(EnableFootIKCurveName_L, FootLockCurveName_L, FootIKBoneName_L,
							FootLock_L_Alpha, FootLock_L_Location, FootLock_L_Rotation);
	SetFootLocking(EnableFootIKCurveName_R, FootLockCurveName_R, FootIKBoneName_R,
							FootLock_R_Alpha, FootLock_R_Location, FootLock_R_Rotation);
	SetFootOffsets(EnableFootIKCurveName_L, FootIKBoneName_L, RootBoneName, FootOffset_L_Location, FootOffset_L_Rotation, FootOffset_L_TargetLocation);
	SetFootOffsets(EnableFootIKCurveName_R, FootIKBoneName_R, RootBoneName, FootOffset_R_Location, FootOffset_R_Rotation, FootOffset_R_TargetLocation);
	SetPelvisOffsets(FootOffset_L_TargetLocation, FootOffset_R_TargetLocation);
}

void UCharacterAnimInst::SetFootLocking(const FName& _EnableFootIKCurve, const FName& _FootLockCurve, const FName& _FootIKBone,
                                        float& CurrentFootLockAlpha, FVector& CurrentFootLockLocation, FRotator& CurrentFootLockRotation)
{
	if (GetCurveValue(_EnableFootIKCurve) <= 0.0f)
	{
		return;
	}
	float FootLockCurveValue = GetCurveValue(_FootLockCurve);
	if (FootLockCurveValue > 0.99f || FootLockCurveValue < CurrentFootLockAlpha)
	{
		CurrentFootLockAlpha = FootLockCurveValue;
	}
	if (CurrentFootLockAlpha > 0.99f)
	{
		const FTransform FootIKBoneTransform = GetOwningComponent()->GetSocketTransform(_FootIKBone, ERelativeTransformSpace::RTS_Component);
		CurrentFootLockLocation = FootIKBoneTransform.GetLocation();
		CurrentFootLockRotation = FootIKBoneTransform.Rotator();
	}
	if (FootLockCurveValue > 0.0f)
	{
		SetFootLockingOffsets(CurrentFootLockLocation, CurrentFootLockRotation);
	}
}

void UCharacterAnimInst::SetFootLockingOffsets(FVector& LocalLocation, FRotator& LocalRotation)
{
	if (!MyCharacter->GetCharacterMovement()->IsMovingOnGround())
	{
		return;
	}
	FRotator RotationDifference = MyCharacter->GetActorRotation() - MyCharacter->GetCharacterMovement()->GetLastUpdateRotation();
	RotationDifference.Normalize();
	FVector LocationDifference = 
	GetOwningComponent()->GetComponentRotation().UnrotateVector((MovementValues.Velocity * UGameplayStatics::GetWorldDeltaSeconds(this)));
	LocalLocation = (LocalLocation - LocationDifference).RotateAngleAxis(RotationDifference.Yaw, FVector(0.0f, 0.0f, -1.0f));
	LocalRotation = (LocalRotation - RotationDifference);
	LocalRotation.Normalize();
}

bool UCharacterAnimInst::IsFlat(FVector FootLOffsetTarget, FVector FootROffsetTarget)
{
	return FMath::Abs(FootLOffsetTarget.Z - FootROffsetTarget.Z) < 1.f;
}

void UCharacterAnimInst::SetFootOffsets(const FName& _EnableFootIKCurve, const FName& _FootIKBone, const FName& Root,
                                        FVector& CurrentFootLocationOffset, FRotator& CurrentFootRotationOffset, FVector& FootOffset_TargetLocation)
{
	if (GetCurveValue(_EnableFootIKCurve) <= 0.0f)
	{
		CurrentFootLocationOffset = {0, 0, 0};
		CurrentFootRotationOffset = {0, 0, 0};
		return;
	}
	/*计算脚步IK位置偏移和旋转偏移*/
	FootOffset_TargetLocation = CurrentFootLocationOffset;
	FRotator LocalFootTargetRotationOffset = CurrentFootRotationOffset;
	
	FVector IKFootFloorLocation = {GetOwningComponent()->GetSocketLocation(_FootIKBone).X, GetOwningComponent()->GetSocketLocation(_FootIKBone).Y,
									GetOwningComponent()->GetSocketLocation(Root).Z};
	FVector StartLocation = {IKFootFloorLocation.X, IKFootFloorLocation.Y, IKFootFloorLocation.Z + IK_TranceDistanceAboveFoot};
	FVector EndLocation = {IKFootFloorLocation.X, IKFootFloorLocation.Y, IKFootFloorLocation.Z - IK_TranceDistanceBelowFoot};
	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle(this, StartLocation, EndLocation, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore,
											EDrawDebugTrace::None, HitResult, true);
	if (MyCharacter->GetCharacterMovement()->IsWalkable(HitResult) && HitResult.bBlockingHit)
	{
		FVector ImpactPoint = HitResult.ImpactPoint;
		FVector ImpactNormal = HitResult.ImpactNormal;
		FootOffset_TargetLocation = (ImpactPoint + ImpactNormal * FootHeight) - (IKFootFloorLocation + FVector(0.f, 0.f , FootHeight));
		float RollOffset = FMath::Atan2(ImpactNormal.Y, ImpactNormal.Z);
		float PitchOffset = FMath::Atan2(ImpactNormal.X, ImpactNormal.Z) * -1.f;
		LocalFootTargetRotationOffset = FRotator(PitchOffset, 0.f, RollOffset);
	}
	/*将计算脚步IK位置偏移和旋转偏移插值过渡*/
	if (FMath::Abs(FootOffset_TargetLocation.Z - CurrentFootLocationOffset.Z) > 2.f)
	{
		CurrentFootLocationOffset = FMath::VInterpTo(CurrentFootLocationOffset, FootOffset_TargetLocation,
								UGameplayStatics::GetWorldDeltaSeconds(this), FootIKOffsetInterpSpeed);
	}
	CurrentFootRotationOffset = FMath::RInterpTo(CurrentFootRotationOffset, LocalFootTargetRotationOffset,
									UGameplayStatics::GetWorldDeltaSeconds(this), FootIKOffsetInterpSpeed);
}

void UCharacterAnimInst::SetPelvisOffsets(FVector FootLOffsetTarget, FVector FootROffsetTarget)
{
	FVector LocalPelvisTargetOffset;
	Pelvis_Alpha = (GetCurveValue(EnableFootIKCurveName_L) + GetCurveValue(EnableFootIKCurveName_R)) / 2.f;
	if (Pelvis_Alpha <= 0 || IsFlat(FootLOffsetTarget, FootROffsetTarget))
	{
		PelvisOffset = {0.f, 0.f, 0.f};
		return;
	}
	FootLOffsetTarget.Z < FootROffsetTarget.Z ? LocalPelvisTargetOffset = FootLOffsetTarget : LocalPelvisTargetOffset = FootROffsetTarget;
	PelvisOffset = FMath::VInterpTo(PelvisOffset, LocalPelvisTargetOffset,
								UGameplayStatics::GetWorldDeltaSeconds(this), FootIKOffsetInterpSpeed);
}

void UCharacterAnimInst::ResetIKOffsets()
{
	FootOffset_L_Location = FMath::VInterpTo(FootOffset_L_Location, FVector(0.f, 0.f, 0.f),
								UGameplayStatics::GetWorldDeltaSeconds(this), FootIKOffsetInterpSpeed);
	FootOffset_R_Location = FMath::VInterpTo(FootOffset_R_Location, FVector(0.f, 0.f, 0.f),
									UGameplayStatics::GetWorldDeltaSeconds(this), FootIKOffsetInterpSpeed);
	FootOffset_L_Rotation = FMath::RInterpTo(FootOffset_L_Rotation, FRotator(0.f, 0.f, 0.f),
								UGameplayStatics::GetWorldDeltaSeconds(this), FootIKOffsetInterpSpeed);
	FootOffset_R_Rotation = FMath::RInterpTo(FootOffset_R_Rotation, FRotator(0.f, 0.f, 0.f),
							UGameplayStatics::GetWorldDeltaSeconds(this), FootIKOffsetInterpSpeed);
	PelvisOffset = FMath::VInterpTo(PelvisOffset, FVector(0.f, 0.f, 0.f),
							UGameplayStatics::GetWorldDeltaSeconds(this), FootIKOffsetInterpSpeed);
}

float UCharacterAnimInst::CalculateLandPrediction()
{
	if (FallSpeed < 200.f)
	{
		return 0.0f;
	}
	UCapsuleComponent* QTCapsuleComponent= MyCharacter->GetCapsuleComponent();
	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;
	FVector FallVelocity = MovementValues.Velocity;
	FallVelocity.Z = FMath::Clamp(FallVelocity.Z, -4000.f, -200.f);
	FallVelocity = FallVelocity.GetUnsafeNormal();
	const float DistanceToTarget = FMath::GetMappedRangeValueClamped(FVector2d(0.f, -4000.f),
									FVector2d(50.f, 2000.f), MovementValues.Velocity.Z);
	UKismetSystemLibrary::CapsuleTraceSingleByProfile(this, QTCapsuleComponent->GetComponentLocation(),
												QTCapsuleComponent->GetComponentLocation() + FallVelocity * DistanceToTarget,
												QTCapsuleComponent->GetUnscaledCapsuleRadius(), QTCapsuleComponent->GetUnscaledCapsuleHalfHeight(),
												CapsuleProfileName, false, ActorsToIgnore, EDrawDebugTrace::None,
												HitResult, true);
	if (MyCharacter->GetCharacterMovement()->IsWalkable(HitResult) && HitResult.bBlockingHit && LandPredictionCurve)
	{
		return FMath::Lerp(GetCurveValue(MaskLandPredictionCurveName), 0.f, LandPredictionCurve->GetFloatValue(HitResult.Time));
	}
	return 0.f;
}

FLeanAmount UCharacterAnimInst::CalculateInAirLeanAmounts()
{
	FVector Velocity = MyCharacter->GetActorRotation().UnrotateVector(MovementValues.Velocity) / 600.f;
	FLeanAmount _InAirLeanAmount;
	_InAirLeanAmount.FB = Velocity.X;
	_InAirLeanAmount.LR = Velocity.Y;
	if (LeanInAirCurve)
	{
		_InAirLeanAmount.FB *= LeanInAirCurve->GetFloatValue(FallSpeed);
		_InAirLeanAmount.LR *= LeanInAirCurve->GetFloatValue(FallSpeed);
	}
	return _InAirLeanAmount;
}

void UCharacterAnimInst::UpdateInAirValues()
{
	FallSpeed = MovementValues.Velocity.Z;
	LandPrediction = CalculateLandPrediction();
	InAirLeanAmount.FB = FMath::FInterpTo(InAirLeanAmount.FB, CalculateInAirLeanAmounts().FB,
					UGameplayStatics::GetWorldDeltaSeconds(MyCharacter), InAirLeanInterpSpeed);
	InAirLeanAmount.LR = FMath::FInterpTo(InAirLeanAmount.LR, CalculateInAirLeanAmounts().LR,
					UGameplayStatics::GetWorldDeltaSeconds(MyCharacter), InAirLeanInterpSpeed);
	ResetIKOffsets();
}

void UCharacterAnimInst::UpdateLayerValues()
{
	BasePose_N = GetCurveValue(BasePoseN_CN);
	BasePose_CLF = GetCurveValue(BasePoseCLF_CN);
	Spine_Add = GetCurveValue(Layering_Spine_Add_CN);
	Head_Add = GetCurveValue(Layering_Head_Add_CN);
	Arm_L_Add = GetCurveValue(Layering_Arm_L_Add_CN);
	Arm_R_Add = GetCurveValue(Layering_Arm_R_Add_CN);
	Hand_R = GetCurveValue(Layering_Hand_R_CN);
	Hand_L = GetCurveValue(Layering_Hand_L_CN);
	Enable_HandIK_L = FMath::Lerp(0.f, GetCurveValue(Enable_HandIK_L_CN), GetCurveValue(Layering_Arm_L_CN));
	Enable_HandIK_R = FMath::Lerp(0.f, GetCurveValue(Enable_HandIK_R_CN), GetCurveValue(Layering_Arm_R_CN));
	Arm_L_LS = GetCurveValue(Layering_Arm_L_LS_CN);
	Arm_L_MS = 1.f - FMath::Floor(Arm_L_LS);
	Arm_R_LS = GetCurveValue(Layering_Arm_R_LS_CN);
	Arm_R_MS = 1.f - FMath::Floor(Arm_R_LS);
}
