// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/EquipmentSlotUI.h"

#include "Components/EquipmentComponent.h"
#include "Components/WrapBox.h"
#include "Core/GameCore/QTPlayerController.h"
#include "Items/ItemTypes/QTItem.h"
#include "UI/GameUI/UMG/ItemSlotUI.h"

void UEquipmentSlotUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UEquipmentSlotUI::OnInventoryChangedEvent(UInventory* Inventory, int32 Index)
{
	if (Inventory->InventoryType != CurrentInventory->InventoryType)
	{
		return;
	}
	UItemSlotUI* ItemSlotUI = Cast<UItemSlotUI>(InventorySlot->GetChildAt(Index));
	if (ItemSlotUI)
	{
		SetItemSlotProperties(ItemSlotUI, Inventory, Index);
	}
}

void UEquipmentSlotUI::SetItemSlotProperties(UItemSlotUI* ItemSlot, UInventory* Inventory, int32 Index)
{
	ItemSlot->SetItemIcon(Inventory->Inventory[Index].Item->ItemIcon);
	ItemSlot->SetItemAmount(Inventory->Inventory[Index].ItemAmounts);
	ItemSlot->SetItemType(Inventory->Inventory[Index].Item->GetItemType());
	ItemSlot->SetItemIndex(Index);
	ItemSlot->InventoryObj = Inventory;
	ItemSlot->SlotType = EQTItemSlotType::EEquipmentSlot;
	ItemSlot->EquipmentUI = EquipmentUI;
}

void UEquipmentSlotUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEquipmentSlotUI::NativeDestruct()
{
	if (AQTPlayerController* PC = GetGamePlayerController())
	{
		if (UEquipmentComponent* InventoryComp = PC->GetEquipmentComp())
			InventoryComp->OnInventoryChanged.Remove(this, "OnInventoryChangedEvent");
	}
	if (AQTPlayerController* PC = GetGamePlayerController())
	{
		PC->CurrentSelectedItemSlot = nullptr;
	}
	Super::NativeDestruct();
}

void UEquipmentSlotUI::InitialSlot()
{
	if (CurrentInventory)
	{
		RefreshInventory();
		if (AQTPlayerController* PC = GetGamePlayerController())
		{
			if (UEquipmentComponent* InventoryComp = PC->GetEquipmentComp())
			{
				InventoryComp->OnInventoryChanged.AddDynamic(this, &UEquipmentSlotUI::OnInventoryChangedEvent);
			}
		}
	}
}

void UEquipmentSlotUI::RefreshInventory()
{
	if (InventorySlot == nullptr || ItemSlotClass == nullptr)
	{
		return;
	}
	InventorySlot->ClearChildren();
	for (int i = 0; i < CurrentInventory->Inventory.Num(); ++i)
	{
		UItemSlotUI* ItemSlot = CreateWidget<UItemSlotUI>(this, ItemSlotClass);
		SetItemSlotProperties(ItemSlot, CurrentInventory, i);
		ItemSlot->InventoryComp = nullptr;
		if (ItemSlot)
		{
			InventorySlot->AddChildToWrapBox(ItemSlot);
		}
	}
}
