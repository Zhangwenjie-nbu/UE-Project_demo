// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/ShortcutBarUI.h"

#include "Components/EquipmentComponent.h"
#include "Core/GameCore/QTAssetManager.h"
#include "Core/GameCore/QTPlayerController.h"
#include "Items/ItemTypes/QTItem.h"
#include "UI/GameUI/UMG/ItemSlotUI.h"

void UShortcutBarUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (UEquipmentComponent* EquipmentComp = GetGamePlayerController()->GetEquipmentComp())
	{
		EquipmentComp->OnShortCutChanged.AddDynamic(this, &UShortcutBarUI::OnShortCutChangedEvent);
	}
	OnShortCutChangedEvent(UQTAssetManager::WeaponItemType);
	OnShortCutChangedEvent(UQTAssetManager::ConsumablesItemType);
}

void UShortcutBarUI::SetItemSlotProperties(UItemSlotUI* ItemSlot, UInventory* Inventory, int32 Index)
{
	ItemSlot->SetItemIcon(Inventory->Inventory[Index].Item->ItemIcon);
	ItemSlot->SetItemAmount(Inventory->Inventory[Index].ItemAmounts);
	ItemSlot->SetItemType(Inventory->Inventory[Index].Item->GetItemType());
	ItemSlot->SetItemIndex(Index);
	ItemSlot->InventoryObj = Inventory;
	ItemSlot->SlotType = EQTItemSlotType::EEquipmentSlot;
}

void UShortcutBarUI::OnShortCutChangedEvent(FPrimaryAssetType _ItemType)
{
	if (UEquipmentComponent* EquipmentComp = GetGamePlayerController()->GetEquipmentComp())
	{
		if (_ItemType == UQTAssetManager::WeaponItemType)
		{
			SetItemSlotProperties(WeaponSlot, EquipmentComp->WeaponInventory, EquipmentComp->CurrentWeaponIndex);
		}
		else if (_ItemType == UQTAssetManager::ConsumablesItemType)
		{
			SetItemSlotProperties(ConsumableSlot, EquipmentComp->ConsumablesInventory, EquipmentComp->CurrentConsumableIndex);
		}
	}
}

