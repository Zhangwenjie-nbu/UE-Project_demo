// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UserWidgetBase.h"
#include "ShortcutBarUI.generated.h"

/**
 * 
 */
class UItemSlotUI;

UCLASS()
class QIONGTU_API UShortcutBarUI : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UItemSlotUI* WeaponSlot;

	UPROPERTY(meta = (BindWidget))
	UItemSlotUI* ConsumableSlot;

protected:
	virtual void NativeOnInitialized() override;

	void SetItemSlotProperties(UItemSlotUI* ItemSlot, UInventory* Inventory, int32 Index);

	UFUNCTION()
	void OnShortCutChangedEvent(FPrimaryAssetType _ItemType);
};

