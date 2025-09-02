// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/QTMovementDelegates.h"
#include "Components/QTComponentBase.h"
#include "EquipmentComponent.generated.h"

/**
 * 
 */
constexpr int32 MaxWeaponSlot = 5;
constexpr int32 MaxArmorSlot = 10;
constexpr int32 MaxConsumablesSlot = 10;

UCLASS()
class QIONGTU_API UEquipmentComponent : public UQTComponentBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	UInventory* ArmorInventory;
	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	UInventory* ConsumablesInventory;
	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	UInventory* WeaponInventory;
	UFUNCTION(BlueprintCallable, Category = Equipment)
	bool SwapInventoryItem(UInventory* Inventory1, UInventory* Inventory2, int Index1, int Index2);

	int32 CurrentWeaponIndex = 0;
	int32 CurrentConsumableIndex = 0;

	FOnInventoryChanged OnInventoryChanged;
	FOnShortcutChanged OnShortCutChanged;

	int32 GetNextValidIndex(UInventory* _Inventory, int32 CurrentIndex);
	void SetCurrentWeapon();
	void SetCurrentConsumable();
protected:
	virtual void BeginPlay() override;
	void InitInventory();


	UPROPERTY(EditDefaultsOnly)
	FPrimaryAssetId EmptyItemID;
};
