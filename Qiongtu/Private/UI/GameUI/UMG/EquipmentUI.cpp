// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/EquipmentUI.h"

#include "Components/EquipmentComponent.h"
#include "Core/GameCore/QTPlayerController.h"
#include "UI/GameUI/UMG/EquipmentSlotUI.h"

void UEquipmentUI::RemoveWidget()
{
	Super::RemoveWidget();
}

void UEquipmentUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (AQTPlayerController* PC = GetGamePlayerController())
	{
		InventoryComp = PC->GetEquipmentComp();
		if (InventoryComp)
		{
			WeaponSlot->CurrentInventory = InventoryComp->WeaponInventory;
			WeaponSlot->EquipmentUI = this;
			WeaponSlot->InitialSlot();
			
			ArmorSlot->CurrentInventory = InventoryComp->ArmorInventory;
			ArmorSlot->EquipmentUI = this;
			ArmorSlot->InitialSlot();
			
			ConsumableSlot->CurrentInventory = InventoryComp->ConsumablesInventory;
			ConsumableSlot->EquipmentUI = this;
			ConsumableSlot->InitialSlot();
		}
	}
}
