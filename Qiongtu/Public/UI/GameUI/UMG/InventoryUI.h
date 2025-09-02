// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UserWidgetBase.h"
#include "InventoryUI.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API UInventoryUI : public UUserWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* WeaponBtn;
	UFUNCTION()
	void OnWeaponBtnClick();

	UPROPERTY(meta = (BindWidget))
	class UButton* EquipmentBtn;
	UFUNCTION()
	void OnEquipmentBtnClick();

	UPROPERTY(meta = (BindWidget))
	class UButton* ConsumablesBtn;
	UFUNCTION()
	void OnConsumablesBtnClick();

	UPROPERTY(meta = (BindWidget))
	class UButton* MaterialBtn;
	UFUNCTION()
	void OnMaterialBtnClick();

	UPROPERTY(meta = (BindWidget))
	class UButton* TaskBtn;
	UFUNCTION()
	void OnTaskBtnClick();

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* InventorySlot;

	UPROPERTY()
	class UInventoryComp* InventoryComp;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UItemSlotUI> ItemSlotClass;
	UPROPERTY(EditDefaultsOnly)
	int32 ColumnNum;

	UFUNCTION()
	void OnInventoryChangedEvent(UInventory* Inventory, int32 Index);

	void SetItemSlotProperties(UItemSlotUI* ItemSlot, UInventory* Inventory, int32 Index);
	
	FPrimaryAssetType CurrentInventoryType;
	void SetCurrentInventoryType(UInventory* Inventory);

public:
	virtual void RemoveWidget() override;

	UFUNCTION()
	void RefreshInventory(UInventory* CurrentInventory);
};

