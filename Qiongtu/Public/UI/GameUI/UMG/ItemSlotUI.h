// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/QTEnums.h"
#include "UI/Core/UserWidgetBase.h"
#include "ItemSlotUI.generated.h"

/**
 * 
 */
class UEquipmentUI;
UCLASS()
class QIONGTU_API UItemSlotUI : public UUserWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	class UImage* ItemIcon;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	class UTextBlock* ItemAmountText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FPrimaryAssetType ItemType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ItemIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ItemAmounts;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* ItemIconTexture;

public:	
	UPROPERTY()
	class UInventoryComp* InventoryComp;
	UPROPERTY()
	class UInventory* InventoryObj;
	UPROPERTY()
	FItemSlot CurrentlySelectedSlot;

	UPROPERTY()
	UEquipmentUI* EquipmentUI = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "ChildWidgetClass")
	TSubclassOf<class UInventoryUI> InventoryUIClass;
	
	UFUNCTION()
	void SetItemIcon(UTexture2D* NewItemIcon);
	UFUNCTION()
	void SetItemAmount(int32 _ItemAmount);
	UFUNCTION()
	void SetItemType(FPrimaryAssetType _ItemType);
	UFUNCTION()
	const FPrimaryAssetType& GetItemType() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool bCanDrag();
	UFUNCTION()
	void SetItemIndex(int32 _ItemIndex);

	UFUNCTION(BlueprintCallable)
	void SwapItemSlot(UItemSlotUI* ItemSlot1, UItemSlotUI* ItemSlot2);
	UFUNCTION(BlueprintCallable)
	void SwapItem(UItemSlotUI* ItemSlot1, UItemSlotUI* ItemSlot2);

	UPROPERTY()
	EQTItemSlotType SlotType;

	UFUNCTION()
	void SetCurrentSelectedItemSlot();

	UFUNCTION(BlueprintCallable)
	void OnLeftMouseButton();
	
};
