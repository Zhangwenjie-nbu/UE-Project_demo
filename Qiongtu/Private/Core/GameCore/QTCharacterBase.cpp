// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/QTCharacterBase.h"

#include "KismetAnimationLibrary.h"
#include "TimerManager.h"
#include "Abilities/Sets/QTAttributeSet.h"
#include "Common/QTBlueprintFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/QTAbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/GameCore/QTAssetManager.h"
#include "Curves/CurveVector.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item/WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayAbilities/Public/GameplayAbilitySpec.h"
#include "Items/ItemTypes/QTConsumablesItem.h"
#include "Items/ItemTypes/QTWeaponItem.h"

// Sets default values
AQTCharacterBase::AQTCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WalkSpeed = 165.0f;
	RunSpeed = 350.0f;
	SprintSpeed = 600.0f;
	OnMovementStatesChanged.AddDynamic(this, &AQTCharacterBase::OnMovementStatesChangedEvent);
	bBreakFall = false;

	AbilitySystemComponent = CreateDefaultSubobject<UQTAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UQTAttributeSet>(TEXT("AttributeSet"));
	
	LockedEnemyUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockedEnemyUI"));
	LockedEnemyUI->SetupAttachment(GetMesh(), FName("spine_02"));

	HealthBarUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarUI"));
	HealthBarUI->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void AQTCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->AddTickPrerequisiteActor(this);
	GaitChanged(MovementStates);
	TArray<FSoftObjectPath> ObjectPaths;
	ObjectPaths.Emplace(MovementDynamicSettingCurvePath);
	UQTBlueprintFunctionLibrary::QTAsyncLoadObject(ObjectPaths);
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddStartupGameplayAbilities();
	}
}

// Called every frame
void AQTCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMovement();
}

// Called to bind functionality to input
void AQTCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AQTCharacterBase::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	switch (GetCharacterMovement()->MovementMode)
	{
		case MOVE_None :
			MovementStates.MovementType = EQTMovementType::ENone;
			break;
		case MOVE_Walking :
			MovementStates.MovementType = EQTMovementType::EGround;
			break;
		case MOVE_NavWalking :
			MovementStates.MovementType = EQTMovementType::EGround;
			break;
		case MOVE_Falling :
			MovementStates.MovementType = EQTMovementType::EFalling;
			break;
		case MOVE_Flying :
			MovementStates.MovementType = EQTMovementType::EAir;
			break;
		case MOVE_Swimming :
			MovementStates.MovementType = EQTMovementType::EWater;
			break;
		case MOVE_Custom : 
			break;
		default: break;
	}
	OnMovementStatesChanged.Broadcast(MovementStates);
}

UAbilitySystemComponent* AQTCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AQTCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		TArray<UAttributeSet*> AttributeSets;
		AttributeSets.Emplace(AttributeSet);
		AbilitySystemComponent->SetSpawnedAttributes(AttributeSets);
	}
}

void AQTCharacterBase::UnPossessed()
{
	Super::UnPossessed();
}

void AQTCharacterBase::OnRep_Controller()
{
	Super::OnRep_Controller();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

void AQTCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AQTCharacterBase, MovementStates);
	DOREPLIFETIME(AQTCharacterBase, MovementValues);
}

void AQTCharacterBase::UpdateMovement()
{
	UpdateMovementValues();
	UpdateMovementDynamicSetting();
	DynamicUpdateGait();
	UpdateCharacterRotation();
}


void AQTCharacterBase::SetMovementValues_Implementation(FMovementValues& _MovementValues)
{
	_MovementValues = MovementValues;
}


void AQTCharacterBase::UpdateMovementValues()
{
	MovementValues.Velocity = GetVelocity();
	MovementValues.Speed = CalculateYawSpeed();
	MovementValues.Acceleration = GetAcceleration();
	MovementValues.AimingRotation = GetControlRotation();
	MovementValues.CharacterRotation = GetActorRotation();
}

float AQTCharacterBase::CalculateYawSpeed()
{
	FVector Vector = GetVelocity();
	Vector.Z = 0.f;
	return Vector.Size();
}


void AQTCharacterBase::GaitChanged(const FMovementStates& NewMovementStates)
{
	if (NewMovementStates.Gait == EQTGait::EWalk)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	else if(NewMovementStates.Gait == EQTGait::ERun)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else if (NewMovementStates.Gait == EQTGait::ESprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AQTCharacterBase::DesireGaitChanged()
{
	SetActualGait();
}

void AQTCharacterBase::SetAllowedGait()
{
	if (MovementStates.Stance == EQTStance::EStance)
	{
		if (MovementStates.RotationMode == EQTRotationMode::EVelocityDirection ||
			MovementStates.RotationMode == EQTRotationMode::ELookingDirection)
		{
			AllowedGait = DesireGait;
			if (DesireGait == EQTGait::ESprint)
			{
				if (FMath::Abs(UKismetAnimationLibrary::CalculateDirection(MovementValues.Velocity, GetActorRotation())) < 70.0f)
				{
					AllowedGait = EQTGait::ESprint;
				}
				else
				{
					AllowedGait = EQTGait::ERun;
				}
			}
		}
		else
		{
			if (DesireGait != EQTGait::ESprint)
			{
				AllowedGait = DesireGait;
			}
		}
	}
	else
	{
		if (DesireGait != EQTGait::ESprint)
		{
			AllowedGait = DesireGait;
		}
	}
}

void AQTCharacterBase::SetActualGait()
{
	SetAllowedGait();
	SetGait(AllowedGait);
}

void AQTCharacterBase::SetGait(EQTGait _Gait)
{
	SetGaitOnServe(_Gait);
}

void AQTCharacterBase::SetGaitOnServe_Implementation(EQTGait _Gait)
{
	SetGaitOnMulticast(_Gait);
}

void AQTCharacterBase::SetGaitOnMulticast_Implementation(EQTGait _Gait)
{
	MovementStates.Gait = _Gait;
	OnMovementStatesChanged.Broadcast(MovementStates);
	GaitChanged(MovementStates);
}


void AQTCharacterBase::DynamicUpdateGait()
{
	if (MovementStates.Gait == EQTGait::ESprint)
	{
		if (FMath::Abs(UKismetAnimationLibrary::CalculateDirection(MovementValues.Velocity, GetActorRotation())) > 70.0f)
		{
			SetGait(EQTGait::ERun);
		}
	}
}

void AQTCharacterBase::OnMovementStatesChangedEvent(const FMovementStates& NewMovementStates)
{
	GaitChanged(NewMovementStates);
}

void AQTCharacterBase::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	MovementStates.Stance = EQTStance::ECrouch;
	OnMovementStatesChanged.Broadcast(MovementStates);
}

void AQTCharacterBase::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	MovementStates.Stance = EQTStance::EStance;
	OnMovementStatesChanged.Broadcast(MovementStates);
}

void AQTCharacterBase::Crouch(bool bClientSimulation)
{
	Super::Crouch(bClientSimulation);
}

void AQTCharacterBase::UnCrouch(bool bClientSimulation)
{
	Super::UnCrouch(bClientSimulation);
}

void AQTCharacterBase::UpdateMovementDynamicSetting()
{
	MovementDynamicSettingCurve = UQTBlueprintFunctionLibrary::QTGetAsyncLoadObject<UCurveVector>(MovementDynamicSettingCurvePath);
	if (!MovementDynamicSettingCurve)
	{
		return;
	}
	const FVector MovementDynamicSettingValue = MovementDynamicSettingCurve->GetVectorValue(GetMappedSpeedValue(MovementValues.Speed));
	GetCharacterMovement()->MaxAcceleration = MovementDynamicSettingValue.X;
	GetCharacterMovement()->BrakingDecelerationWalking = MovementDynamicSettingValue.Y;
	GetCharacterMovement()->GroundFriction = MovementDynamicSettingValue.Z;
}

float AQTCharacterBase::GetMappedSpeedValue(float _Speed)
{
	float MappedValue;
	if (_Speed <= WalkSpeed + 10)
	{
		MappedValue = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, WalkSpeed),
			FVector2D(0.0f, 1.0f), _Speed);
	}
	else if (_Speed <= RunSpeed + 10)
	{
		MappedValue = MappedValue = FMath::GetMappedRangeValueClamped(FVector2D(WalkSpeed, RunSpeed),
			FVector2D(1.0f, 2.0f), _Speed);
	}
	else
		MappedValue = MappedValue = FMath::GetMappedRangeValueClamped(FVector2D(RunSpeed, SprintSpeed),
			FVector2D(2.0f, 3.0f), _Speed);
	return MappedValue;
}

FVector AQTCharacterBase::GetAcceleration()
{
	const FVector TempAcceleration = (GetVelocity() - PreVelocity) / UGameplayStatics::GetWorldDeltaSeconds(this);
	PreVelocity = GetVelocity();
	return TempAcceleration;
}

float AQTCharacterBase::CalculateCharacterRotationInterpSpeed() 
{
	float InterpSpeed = FMath::Abs((GetControlRotation().Yaw - PreAimYawRate) / UGameplayStatics::GetWorldDeltaSeconds(this));
	InterpSpeed = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 300.0f), FVector2D(1.0f, 3.0f), InterpSpeed);
	if (!NormalRotationCurvePath.IsValid())
	{
		InterpSpeed *= 8;
	}
	else
	{
		NormalRotationCurve = UQTBlueprintFunctionLibrary::QTSyncLoadObject<UCurveFloat>(NormalRotationCurvePath);
		if (NormalRotationCurve)
		{
			InterpSpeed *= NormalRotationCurve->GetFloatValue(GetMappedSpeedValue(MovementValues.Speed));
		}
		else
			InterpSpeed *= 8;
	}
	PreAimYawRate = GetControlRotation().Yaw;
	return FMath::Abs(InterpSpeed);
}

//平滑设置角色旋转
void AQTCharacterBase::SmoothCharacterRotation(FRotator _TargetRotation, float _TargetRotationInterpSpeed,
	float _CharacterRotationInterpSpeed)
{
	_TargetRotation = FRotator(GetActorRotation().Pitch, _TargetRotation.Yaw, GetActorRotation().Roll);
	//将目标旋转插值过渡
	TargetRotation = FMath::RInterpConstantTo(TargetRotation, _TargetRotation,
					UGameplayStatics::GetWorldDeltaSeconds(this), _TargetRotationInterpSpeed);
	//将当前旋转向目标旋转插值过渡，并将结果设置为角色旋转。
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), _TargetRotation, UGameplayStatics::GetWorldDeltaSeconds(this),
		_CharacterRotationInterpSpeed));
}

void AQTCharacterBase::UpdateCharacterRotation()
{
	FRotator _TargetRotation;
	float _CharacterRotationInterpSpeed = CalculateCharacterRotationInterpSpeed();
	float TargetRotationInterpSpeed;
	if(MovementStates.RotationMode == EQTRotationMode::ELookingDirection)
	{
		if (MovementStates.Gait == EQTGait::ESprint)
			_TargetRotation = GetVelocity().ToOrientationRotator();
		else
			_TargetRotation = GetControlRotation();
		TargetRotationInterpSpeed = 500.0f;
	}
	else if (MovementStates.RotationMode == EQTRotationMode::EAiming)
	{
		_TargetRotation = GetControlRotation();
		TargetRotationInterpSpeed = 500.0f;
	}
	else
	{
		_TargetRotation = GetVelocity().ToOrientationRotator();
		TargetRotationInterpSpeed = 700.0f;
	}
	//若不设置在有移动或者移动输入时才更新，那么自由视角移动（速度方向移动）在角色停下来来时会将角色旋转强制转到正前方
	//若不设置当旋转模式为控制器方向时更新旋转，会导致角色停下后不旋转视角角色不更新旋转。
	if ((IsMoving() && HasMovementInput()) || MovementStates.RotationMode == EQTRotationMode::ELookingDirection)
	{
		if (bCanRotate)
		{
			SmoothCharacterRotation(_TargetRotation, TargetRotationInterpSpeed, _CharacterRotationInterpSpeed);
		}	
	}
}

void AQTCharacterBase::SetRotationMode(EQTRotationMode _NewRotationMode)
{
	if (_NewRotationMode == MovementStates.RotationMode)
	{
		return;
	}
	MovementStates.RotationMode = _NewRotationMode;
}

bool AQTCharacterBase::IsMoving()
{
	FVector Vector = GetVelocity();
	Vector.Z = 0.0f;
	return Vector.Size() > 20.0f;
}

bool AQTCharacterBase::HasMovementInput()
{
	return (MovementValues.Acceleration.Size() / GetCharacterMovement()->GetMaxAcceleration()) > 0.0f;
}

void AQTCharacterBase::Jump()
{
	if (MovementStates.MovementType == EQTMovementType::ENone || MovementStates.MovementType == EQTMovementType::EGround)
	{
		if (MovementStates.Stance == EQTStance::EStance)
		{
			Super::Jump();
			MovementValues.bJumped = true;
		}
		else
		{
			UnCrouch();
		}
	}
}

void AQTCharacterBase::StopJumping()
{
	Super::StopJumping();
	MovementValues.bJumped = false;
}

void AQTCharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if (bBreakFall)
	{
		//播放翻滚动画等等
	}
	else
	{
		GetWorldTimerManager().ClearTimer(BreakingFrictionTimerHandle);
		if (HasMovementInput())
		{
			//落地时还在移动，设置制动摩擦力小
			GetCharacterMovement()->BrakingFrictionFactor = 0.5f; 
		}
		else
		{
			GetCharacterMovement()->BrakingFrictionFactor = 3.0f; 
		}
		GetWorldTimerManager().SetTimer(BreakingFrictionTimerHandle, this,
												&AQTCharacterBase::BreakingFrictionCheck, 0.5f, false);
	}
}

void AQTCharacterBase::BreakingFrictionCheck()
{
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f; 
}

bool AQTCharacterBase::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation)
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, bAllowRemoteActivation);
	}
	return false;
}


void AQTCharacterBase::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags,
	TArray<UQTGameplayAbility*>& ActiveAbilities)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
	}
}

void AQTCharacterBase::K2_ActiveSkill(TSubclassOf<UGameplayAbility> AbilityClass)
{
	ActiveSkill(AbilityClass);
}

void AQTCharacterBase::ActiveSkill_Implementation(TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (AbilitySystemComponent == nullptr)
	{
		return;
	}
	AbilitySystemComponent->TryActivateAbilityByClass(AbilityClass);
}

void AQTCharacterBase::SetWeaponCollision(ECollisionEnabled::Type CollisionType)
{
	if (CurrentWeapon == nullptr)
	{
		return;
	}
	CurrentWeapon->DefenseBoxComp->SetCollisionEnabled(CollisionType);
}

void AQTCharacterBase::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent);
	if (!bAbilitiesInitialized)
	{
		// Grant abilities, but only on the server	
		for (const TSubclassOf<UGameplayAbility>& StartupAbility : GameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, INDEX_NONE, this));
		}

		// // Now apply passives
		// for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
		// {
		// 	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		// 	EffectContext.AddSourceObject(this);
		//
		// 	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		// 	if (NewHandle.IsValid())
		// 	{
		// 		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		// 	}
		// }

		bAbilitiesInitialized = true;
	}
}

void AQTCharacterBase::RemoveStartupGameplayAbilities()
{

}

EQTDodge AQTCharacterBase::GetCurrentDodgeDirection()
{
	return EQTDodge::EForwardDodge;
}

void AQTCharacterBase::HandleHealthChangedOnMulticast_Implementation(float CurrentValue, float MaxValue)
{
	SetHealthBarPercent(CurrentValue, MaxValue);
}

void AQTCharacterBase::SetOverlayState(const EQTOverlayStates& NewOverlayState)
{
	if (NewOverlayState == MovementStates.OverlayState)
	{
		return;
	}
	PreOverlayState = MovementStates.OverlayState;
	MovementStates.OverlayState = NewOverlayState;
	OnOverlayStateChanged();
}

void AQTCharacterBase::OnOverlayStateChanged()
{
	OnMovementStatesChanged.Broadcast(MovementStates);
}

void AQTCharacterBase::AttachHeldItem(AItemBase* const HeldItem, const FName& AttachSocketName, bool bChangeOverlayState)
{
	if (!HeldItem)
	{
		return;
	}
	HeldItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachSocketName);
	if (HeldItem->ChangeOverlayStateItemType == EQTOverlayStates::ENone || !bChangeOverlayState)
	{
		return;
	}
	SetOverlayState(HeldItem->ChangeOverlayStateItemType);
}



void AQTCharacterBase::SetWeaponVisibility(AWeaponBase* CurrentPositionWeapon, bool bVisible)
{
	if (!CurrentPositionWeapon)
	{
		return;
	}
	CurrentPositionWeapon->SetHidden(bVisible);
	CurrentPositionWeapon = nullptr;
}

void AQTCharacterBase::SwitchWeapon(AWeaponBase* NewWeapon)
{
	//战斗状态切换右手武器
}


void AQTCharacterBase::SetCurrentWeapon(AWeaponBase* NewWeapon)
{
	if (NewWeapon)
	{
		NewWeapon->PickUpSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (CurrentWeapon)
		{
			CurrentWeapon->Destroy();
		}
		CurrentWeapon = NewWeapon;
		CurrentWeapon->SetOwner(this);
		AttachItemToSlot(UQTAssetManager::WeaponItemType);
		InitAGRCombat();
	}
}

void AQTCharacterBase::SetCurrentConsumable(UQTConsumablesItem* ConsumableItem)
{
	if (ConsumableItem)
	{
		CurrentConsumableClass = ConsumableItem->ConsumablesClass;
	}
}

void AQTCharacterBase::UpdateCurrentItem(FItemSlot& NewItemSlot)
{
	if (const UQTWeaponItem* NewWeapon = Cast<UQTWeaponItem>(NewItemSlot.Item))
	{
		if (CurrentWeapon != nullptr && CurrentWeapon->ItemAssetID == NewWeapon->GetPrimaryAssetId())
		{
			CurrentWeapon->ItemAttributeData = NewItemSlot.ItemData;
			return;
		}
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z = SpawnLocation.Z + 200.f;
		SpawnLocation.X = SpawnLocation.X - 200.f;
		AWeaponBase* _CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(NewWeapon->WeaponClass, SpawnLocation, FRotator());
		SetCurrentWeapon(_CurrentWeapon);
	}
	if (UQTConsumablesItem* NewConsumable = Cast<UQTConsumablesItem>(NewItemSlot.Item))
	{
		SetCurrentConsumable(NewConsumable);
	}
}

void AQTCharacterBase::AttachItemToSlot(FPrimaryAssetType ItemType)
{
	if (ItemType == UQTAssetManager::WeaponItemType)
	{
		if (CombatState == EQTCombatStates::EIdle)
		{
			if (CurrentWeapon->WeaponType == EQTWeaponType::ELightWeapon)
			{
				AttachHeldItem(CurrentWeapon, LeftHandSocketName, false);
			}
			else
			{
				AttachHeldItem(CurrentWeapon, BackHandSocketName, false);
			}
		}
		else
		{
			AttachHeldItem(CurrentWeapon, RightHandSocketName, true);
		}
	}
}

void AQTCharacterBase::DrawWeapon()
{
	if (CurrentWeapon && CombatState == EQTCombatStates::EIdle)
	{
		AttachHeldItem(CurrentWeapon, RightHandSocketName, true);
		CombatState = EQTCombatStates::ECombat;
	}
}

void AQTCharacterBase::RecoveryWeapon()
{
	if (CurrentWeapon && CombatState == EQTCombatStates::ECombat)
	{
		AttachHeldItem(CurrentWeapon, LeftHandSocketName, false);
		CombatState = EQTCombatStates::EIdle;
		SetOverlayState(EQTOverlayStates::EDefault);
	}
}

bool AQTCharacterBase::CalculateDesireChuJueAnim()
{
	if (LockedTargetEnemy == nullptr)
	{
		DesireChuJueAnim = EQTChuJueAnim::ENone;
		return false;
	}
	if (FMath::Abs(UKismetAnimationLibrary::CalculateDirection(GetActorLocation() - LockedTargetEnemy->GetActorLocation(),
		LockedTargetEnemy->GetActorRotation())) > 150.f)
	{
		if (LockedTargetEnemy != nullptr && LockedTargetEnemy->CharacterBaseState == EQTBaseState::EIdle)
		{
			DesireChuJueAnim = EQTChuJueAnim::EAnSha;
			return true;
		}
	}
	return false;
}

void AQTCharacterBase::HandleHealthChangedOnServer_Implementation(float Health, float MaxHealth)
{
	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		HandleHealthChangedOnMulticast(Health, MaxHealth);
	}
}

