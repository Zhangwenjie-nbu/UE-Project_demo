// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UserWidgetBase.h"
#include "EquipmentUI.generated.h"

/**
 * 
 */
class UEquipmentSlotUI;

UCLASS()
class QIONGTU_API UEquipmentUI : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UEquipmentSlotUI* WeaponSlot;
	UPROPERTY(meta = (BindWidget))
	UEquipmentSlotUI* ArmorSlot;
	UPROPERTY(meta = (BindWidget))
	UEquipmentSlotUI* ConsumableSlot;
	UPROPERTY()
	class UEquipmentComponent* InventoryComp;

	virtual void RemoveWidget() override;

protected:
	virtual void NativeOnInitialized() override;
};
