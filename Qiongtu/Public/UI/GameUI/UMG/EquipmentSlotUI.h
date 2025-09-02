// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UserWidgetBase.h"
#include "EquipmentSlotUI.generated.h"

/**
 * 
 */
class UEquipmentUI;

UCLASS()
class QIONGTU_API UEquipmentSlotUI : public UUserWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* InventorySlot;
public:
	UPROPERTY(EditDefaultsOnly)
	UInventory* CurrentInventory;
	UPROPERTY(EditDefaultsOnly)
	FPrimaryAssetType ItemType;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UItemSlotUI> ItemSlotClass;

	UFUNCTION()
	void OnInventoryChangedEvent(UInventory* Inventory, int32 Index);

	void SetItemSlotProperties(UItemSlotUI* ItemSlot, UInventory* Inventory, int32 Index);

public:
	UPROPERTY()
	UEquipmentUI* EquipmentUI = nullptr;

	
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	void InitialSlot();
	
	UFUNCTION()
	void RefreshInventory();

};
