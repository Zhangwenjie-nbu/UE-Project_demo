// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Common/QTStructs.h"
#include "CharacterAnimInst.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API UCharacterAnimInst : public UAnimInstance
{
	GENERATED_BODY()
protected:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperty)
	void UpdateAnimationProperties();

	UPROPERTY(BlueprintReadWrite, Category = "CharacterInfo")
	class AQTCharacterBase* MyCharacter;
	
	UPROPERTY(BlueprintReadWrite, Category = "QTMovement")
	FMovementValues MovementValues;

	UPROPERTY(BlueprintReadWrite, Category = "QTMovement")
	FMovementStates MovementStates;

	void UpdateLocalMovementValues();

//运动状态的更新委托函数
	UFUNCTION()
	void OnMovementStatesChangedEvent(const FMovementStates& NewMovementStates);

//走跑的步幅步态
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MovementCurve)
	UCurveFloat* WalkStrideCurve;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MovementCurve)
	UCurveFloat* RunStrideCurve;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MovementCurve)
	UCurveFloat* CrouchStrideCurve;
	UPROPERTY(EditDefaultsOnly, Category = MovementCurve)
	FName WeightGaitCurveName;
	UPROPERTY(BlueprintReadOnly, Category = MovementBlendValue)
	float StrideBlend;
	UPROPERTY(BlueprintReadOnly, Category = MovementBlendValue)
	float WalkRunBlend;
	UPROPERTY(BlueprintReadOnly, Category = MovementBlendValue)
	float PlayRate;
	UPROPERTY(BlueprintReadOnly, Category = MovementBlendValue)
	float CrouchPlayRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ConfigData)
	float StrideBlendInterpSpeed = 0.2f;
	float CalculateStrideBlend();
	float CalculateWalkRunBlend();
	float CalculatePlayRate();
	float CalculateCrouchingPlayRate();
	void UpdateMoveBlendValues();

//冲刺混合
	UPROPERTY(BlueprintReadOnly, Category = MovementBlendValue)
	FVector RelativeAcceleration;
	FVector CalculateRelativeAcceleration();

//速度混合
	UPROPERTY(BlueprintReadOnly, Category = MovementBlendValue)
	FVelocityBlend VelocityBlend;
	void SetVelocityBlend();
	FVector VelocityDir;
	UPROPERTY(EditDefaultsOnly, Category = ConfigData)
	float VBInterpSpeed = 12.0f;

//八向移动
	//移动方向
	UPROPERTY(BlueprintReadOnly, Category = MovementBlendValue)
	EQTMovementDirection MoveDirection;
	void CalculateMovementDirection();
	UPROPERTY(EditDefaultsOnly, Category = ConfigData)
	float FL_Threshold;
	UPROPERTY(EditDefaultsOnly, Category = ConfigData)
	float FR_Threshold;
	UPROPERTY(EditDefaultsOnly, Category = ConfigData)
	float BL_Threshold;
	UPROPERTY(EditDefaultsOnly, Category = ConfigData)
	float BR_Threshold;

//八向移动混合设置
	UPROPERTY(BlueprintReadWrite, Category = MovementBlendValue)
	EQTHipsDirection HipsDirection;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MovementBlendValue)
	bool IsPivot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ConfigData)
	float TriggerPivotLimitSpeed = 200.0f;

//倾斜姿势
	UPROPERTY(BlueprintReadOnly, Category = LeanSystem)
	FLeanAmount GroundedLeanAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ConfigData)
	float GroundLeanInterpSpeed = 4.0f;
	void SetLeanAmount(float _FB, float _LR);
	void InterpLeanAmount();
	void UpdataLeanAmount();

//是否移动与是否有移动输入
	UPROPERTY(BlueprintReadOnly, Category = MovementConditionValue)
	bool IsMoving = false;
	UPROPERTY(BlueprintReadOnly, Category = MovementConditionValue)
	bool HasMoveInput = false;
	void UpdateMovementConditionValue();

//脚步锁定
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	float FootLock_L_Alpha;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	float FootLock_R_Alpha;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	FVector FootLock_L_Location;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	FVector FootLock_R_Location;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	FRotator FootLock_L_Rotation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	FRotator FootLock_R_Rotation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	FName EnableFootIKCurveName_L;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	FName FootLockCurveName_L;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	FName FootIKBoneName_L;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	FName EnableFootIKCurveName_R;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	FName FootLockCurveName_R;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	FName FootIKBoneName_R;
	void UpdateFootIKValues();
	void SetFootLocking(const FName& _EnableFootIKCurve, const FName& _FootLockCurve, const FName& _FootIKBone,
								float& CurrentFootLockAlpha, FVector& CurrentFootLockLocation, FRotator& CurrentFootLockRotation);
	void SetFootLockingOffsets(FVector& LocalLocation, FRotator& LocalRotation);

//脚步IK
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ConfigData)
	float FootHeight = 13.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ConfigData)
	float IK_TranceDistanceAboveFoot = 50.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ConfigData)
	float IK_TranceDistanceBelowFoot = 45.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	FVector FootOffset_L_Location;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	FVector FootOffset_R_Location;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	FRotator FootOffset_L_Rotation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	FRotator FootOffset_R_Rotation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	float Pelvis_Alpha;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	FVector PelvisOffset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ConfigData)
	float FootIKOffsetInterpSpeed = 30.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FootIK)
	FName RootBoneName;
	bool IsFlat(FVector FootLOffsetTarget, FVector FootROffsetTarget);
	FVector FootOffset_L_TargetLocation;
	FVector FootOffset_R_TargetLocation;

	void SetFootOffsets(const FName& _EnableFootIKCurve, const FName& _FootIKBone, const FName& Root,
								FVector& CurrentFootLocationOffset, FRotator& CurrentFootRotationOffset, FVector& FootOffset_TargetLocation);
	void SetPelvisOffsets(FVector FootLOffsetTarget, FVector FootROffsetTarget);
	void ResetIKOffsets();

//跳跃
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Jump)
	float JumpPlayRate = 1.2f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Jump)
	float FallSpeed = 1.2f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Jump)
	float LandPrediction = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MovementCurve)
	UCurveFloat* LeanInAirCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MovementCurve)
	UCurveFloat* LandPredictionCurve;
	UPROPERTY(EditDefaultsOnly, Category = VariableNames)
	FName CapsuleProfileName;
	UPROPERTY(EditDefaultsOnly, Category = VariableNames)
	FName MaskLandPredictionCurveName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ConfigData)
	float InAirLeanInterpSpeed = 4.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LeanSystem)
	FLeanAmount InAirLeanAmount;
	float CalculateLandPrediction();
	FLeanAmount CalculateInAirLeanAmounts();
	void UpdateInAirValues();

//混合动画层
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	float BasePose_N = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	float BasePose_CLF = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	float Arm_L = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	float Arm_L_Add = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	float Arm_L_LS = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	float Arm_L_MS = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	float Arm_R = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	float Arm_R_Add = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	float Arm_R_LS = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	float Arm_R_MS = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	float Hand_L = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	float Hand_R = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	float Legs_Add = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	float Spine_Add = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	float Head_Add = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	float Enable_HandIK_L = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	float Enable_HandIK_R = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	FName BasePoseN_CN = "BasePose_N";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	FName BasePoseCLF_CN = "BasePose_CLF";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	FName Layering_Spine_Add_CN = "Layering_Spine_Add";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	FName Layering_Head_Add_CN = "Layering_Head_Add";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	FName Layering_Arm_L_Add_CN = "Layering_Arm_L_Add";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	FName Layering_Arm_R_Add_CN = "Layering_Arm_R_Add";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	FName Layering_Hand_R_CN = "Layering_Hand_R";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	FName Layering_Hand_L_CN = "Layering_Hand_L";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	FName Enable_HandIK_L_CN = "Enable_HandIK_L";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	FName Layering_Arm_L_CN = "Layering_Arm_L";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	FName Enable_HandIK_R_CN = "Enable_HandIK_R";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	FName Layering_Arm_R_CN = "Layering_Arm_R";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	FName Layering_Arm_L_LS_CN = "Layering_Arm_L_LS";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = LayerBlending)
	FName Layering_Arm_R_LS_CN = "Layering_Arm_R_LS";

	void UpdateLayerValues();
};
