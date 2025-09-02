// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/QTMovementDelegates.h"
#include "Common/QTMovementInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "QTCharacterBase.generated.h"

class UQTConsumablesItem;
class AWeaponBase;
class UWidgetComponent;
UCLASS()
class QIONGTU_API AQTCharacterBase : public ACharacter, public IQTMovementInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AQTCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_Controller() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	//Movement System
	FVector PreVelocity;
	float PreAimYawRate;
	FRotator TargetRotation;
	FTimerHandle BreakingFrictionTimerHandle;
	
	UFUNCTION(BlueprintCallable)
	void UpdateMovement();

	//运动数据及状态的结构体以及将其传递给动画蓝图的接口	
	UPROPERTY(BlueprintReadWrite, Category = "QTMovementValues", Replicated)
	FMovementValues MovementValues;
	virtual void SetMovementValues_Implementation(FMovementValues& _MovementValues) override;
	UPROPERTY(BlueprintReadWrite, Category = "QTMovementStates", Replicated)
	FMovementStates MovementStates;

	//运动数据及状态更新函数
	UFUNCTION(BlueprintCallable)
	void UpdateMovementValues();
	float CalculateYawSpeed();

	//步态切换
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "QTMovementValues")
	float WalkSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "QTMovementValues")
	float RunSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "QTMovementValues")
	float SprintSpeed;
	UPROPERTY(BlueprintReadWrite, Category = "QTMovementStates")
	EQTGait DesireGait;
	UPROPERTY(BlueprintReadWrite, Category = "QTMovementStates")
	EQTGait AllowedGait;

	//运动状态使用委托更新到动画蓝图
	FOnMovementStatesChanged OnMovementStatesChanged;
	void GaitChanged(const FMovementStates& NewMovementStates);
	
	UFUNCTION(BlueprintCallable, Category = "QTMovementStates")
	void DesireGaitChanged();
	
	void SetAllowedGait();
	void SetActualGait();

	void SetGait(EQTGait _Gait);
	UFUNCTION(Server, Reliable)
	void SetGaitOnServe(EQTGait _Gait);
	UFUNCTION(NetMulticast, Reliable)
	void SetGaitOnMulticast(EQTGait _Gait);
	void DynamicUpdateGait();
	UFUNCTION()
	void OnMovementStatesChangedEvent(const FMovementStates& NewMovementStates);

	//蹲伏逻辑
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void Crouch(bool bClientSimulation = false);
	virtual void UnCrouch(bool bClientSimulation = false);

	//动态设置最大加速度、制动速度以及摩擦力
	UPROPERTY()
	UCurveVector* MovementDynamicSettingCurve;
	UPROPERTY(EditDefaultsOnly, Category = MovementCurve, meta = (AllowedClasses = "CurveVector"))
	FSoftObjectPath MovementDynamicSettingCurvePath;
	void UpdateMovementDynamicSetting();
	float GetMappedSpeedValue(float _Speed);

	//计算当前加速度
	FVector Acceleration;
	FVector GetAcceleration();

	//角色旋转
	float CalculateCharacterRotationInterpSpeed();
	UPROPERTY()
	UCurveFloat* NormalRotationCurve;
	UPROPERTY(EditDefaultsOnly, Category = MovementCurve, meta = (AllowedClasses = "CurveFloat"))
	FSoftObjectPath NormalRotationCurvePath;
	void SmoothCharacterRotation(FRotator _TargetRotation, float _TargetRotationInterpSpeed, float _CharacterRotationInterpSpeed);
	void UpdateCharacterRotation();
	void SetRotationMode(EQTRotationMode _NewRotationMode);

	//判断是否移动以及有按键输入
	bool IsMoving();
	bool HasMovementInput();

	UPROPERTY(BlueprintReadWrite)
	bool bCanRotate = true;
	
	//跳跃逻辑
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Jump)
	bool bBreakFall;
	virtual void Jump() override;
	virtual void StopJumping() override;
	//处理落地翻滚
	virtual void Landed(const FHitResult& Hit) override; 
	void BreakingFrictionCheck();

//技能系统
protected:
	UPROPERTY(BlueprintReadOnly)
	class UQTAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY()
	class UQTAttributeSet* AttributeSet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<class UGameplayAbility>> GameplayAbilities;

	bool bAbilitiesInitialized = false;
public:
	UFUNCTION(BlueprintCallable)
	bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation);
	UFUNCTION(BlueprintCallable)
	void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<class UQTGameplayAbility*>& ActiveAbilities);
	
	UFUNCTION(BlueprintCallable, DisplayName = "ActiveSkill", Category = Abilities)
	void K2_ActiveSkill(TSubclassOf<UGameplayAbility> AbilityClass);
	UFUNCTION(Server, Reliable) 
	void ActiveSkill(TSubclassOf<UGameplayAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category = Abilities)
	void SetWeaponCollision(ECollisionEnabled::Type CollisionType);
	
	void AddStartupGameplayAbilities();

	void RemoveStartupGameplayAbilities();
	UFUNCTION(Server, Reliable)
	void HandleHealthChangedOnServer(float Health, float MaxHealth);
	UFUNCTION(NetMulticast, Reliable)
	void HandleHealthChangedOnMulticast(float CurrentValue, float MaxValue);
	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void SetHealthBarPercent(float CurrentValue, float MaxValue);

	UPROPERTY(BlueprintReadWrite, Category = Abilities)
	TMap<AQTCharacterBase*, bool> BlockedCharacters;

	//判断当前是否格挡以区分启动攻击碰撞还是格挡碰撞
	UPROPERTY(BlueprintReadWrite, Category = Abilities)
	bool bBlocking = false;

	//闪避
	float RightValue = 0.f;
	float ForwardValue = 0.f;

	UFUNCTION(BlueprintCallable, Category = LockedSystem)
	virtual EQTDodge GetCurrentDodgeDirection();

	UPROPERTY(BlueprintReadOnly)
	EQTDodge CurrentDodgeDirection;

	//锁定
	UPROPERTY(BlueprintReadOnly, Category = LockedSystem)
	AQTCharacterBase* LockedTargetEnemy = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = LockedSystem)
	UWidgetComponent* LockedEnemyUI;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = LockedSystem)
	UWidgetComponent* HealthBarUI;

//覆盖层
	UPROPERTY(BlueprintReadWrite);
	EQTOverlayStates PreOverlayState;
	
	UFUNCTION(BlueprintCallable)
	void SetOverlayState(const EQTOverlayStates& NewOverlayState);
	
	void OnOverlayStateChanged();
	//其他物品
	void AttachHeldItem(class AItemBase* const HeldItem, const FName& AttachSocketName, bool bChangeOverlayState);
	//武器
	void SetWeaponVisibility(AWeaponBase* CurrentPositionWeapon, bool bVisible);
	void SwitchWeapon(AWeaponBase* NewWeapon);

//角色武器、物品
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
	FName LeftHandSocketName = "LeftWeapon";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
	FName RightHandSocketName = "RightWeapon";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
	FName BackHandSocketName = "BackWeapon";

public:
	UPROPERTY(BlueprintReadOnly)
	EQTCombatStates CombatState = EQTCombatStates::EIdle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
	AWeaponBase* CurrentWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
	class AConsumablesBase* CurrentConsumable;
	
	UPROPERTY(BlueprintReadWrite, Category = Item)
	TSubclassOf<class AConsumablesBase> CurrentConsumableClass;

	UFUNCTION(BlueprintCallable, Category = Item)
	virtual void SetCurrentWeapon(AWeaponBase* NewWeapon);

	void SetCurrentConsumable(UQTConsumablesItem* ConsumableItem);
	
	UFUNCTION(BlueprintCallable, Category = Item)
	void UpdateCurrentItem(FItemSlot& NewItemSlot);

	void AttachItemToSlot(FPrimaryAssetType ItemType);

	UFUNCTION(BlueprintCallable)
	void DrawWeapon();
	UFUNCTION(BlueprintCallable)
	void RecoveryWeapon();

//攻击检测
	UFUNCTION(BlueprintImplementableEvent)
	void InitAGRCombat();

//处决
	UFUNCTION(BlueprintCallable, Category = Abilities)
	bool CalculateDesireChuJueAnim();

	UPROPERTY(BlueprintReadWrite)
	EQTChuJueAnim DesireChuJueAnim;
	
	UPROPERTY(BlueprintReadWrite)
	EQTBaseState CharacterBaseState = EQTBaseState::EIdle;
};
