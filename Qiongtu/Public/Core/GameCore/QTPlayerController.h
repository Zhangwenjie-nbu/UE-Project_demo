// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "QTPlayerController.generated.h"

/**
 * 
 */
class UItemSlotUI;
UCLASS()
class QIONGTU_API AQTPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AQTPlayerController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	float TurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	float LookUpRate;
	const float BaseRate = 50.0f;

	UPROPERTY()
	class AQTCharacterBase* MyPawn = nullptr;
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY()
	class UUserWidgetBase* CurrentWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	class UInventoryComp* InventoryComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	class UEquipmentComponent* EquipmentComp;
	UPROPERTY(EditDefaultsOnly, Category = ChildWidgetClass)
	TSubclassOf<class UMainGameSettingsUI> MainGameSettingsWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = ChildWidgetClass)
	TSubclassOf<class UUserWidgetBase> PlayerHUDClass;
	void SetPlayerHUD();
	
	bool bInventoryOpened;

	void Turn(float Value);
	void LookUp(float Value);
	void PickUpItem();
	void SwitchCurrentWeapon();
	void SwitchCurrentConsumable();
public:
	void ESCPressed();
protected:
	void LockedEnemy();
	void LeftLocked();
	void RightLocked();
	
	UPROPERTY()
	TArray<class AActor*> LockedEnemies;
	UPROPERTY(EditAnywhere, Category = LockedSystem)
	float LockedDistance = 1000.f;
	void GetRecentlyRenderedEnemy();
	class AQTCharacterBase* FindForwardEnemy();
	class AQTCharacterBase* FindLeftEnemy();
	class AQTCharacterBase* FindRightEnemy();
	void LockEnemy(class AQTCharacterBase* Enemy);
	

	void SetLockSystem();
	UFUNCTION(BlueprintImplementableEvent, Category = LockedSystem)
	void SetTargetLockUIVisible(class AQTCharacterBase* Target, bool bVisible);

public:
	UFUNCTION(BlueprintCallable)
	void ChangeWidget(const TSubclassOf<UUserWidgetBase> NewWidgetClass);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	class UInventoryComp* GetInventoryComp();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	class UEquipmentComponent* GetEquipmentComp();

	virtual void SetupInputComponent() override;

	UPROPERTY()
	UItemSlotUI* CurrentSelectedItemSlot;
};
