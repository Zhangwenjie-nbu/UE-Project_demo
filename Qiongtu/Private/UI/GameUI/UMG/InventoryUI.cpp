// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/InventoryUI.h"
#include "Components/Button.h"
#include "Components/InventoryComp.h"
#include "Components/UniformGridPanel.h"
#include "Core/GameCore/QTAssetManager.h"
#include "Core/GameCore/QTPlayerController.h"
#include "Items/ItemTypes/QTItem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/GameUI/UMG/ItemSlotUI.h"


void UInventoryUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	WeaponBtn->OnClicked.AddDynamic(this, &UInventoryUI::OnWeaponBtnClick);
	EquipmentBtn->OnClicked.AddDynamic(this, &UInventoryUI::OnEquipmentBtnClick);
	ConsumablesBtn->OnClicked.AddDynamic(this, &UInventoryUI::OnConsumablesBtnClick);
	MaterialBtn->OnClicked.AddDynamic(this, &UInventoryUI::OnMaterialBtnClick);
	TaskBtn->OnClicked.AddDynamic(this, &UInventoryUI::OnTaskBtnClick);
	
	if (AQTPlayerController* PC = GetGamePlayerController())
	{
		InventoryComp = PC->GetInventoryComp();
		if (InventoryComp)
		{
			UInventory* CurrentInventory = InventoryComp->GetWeaponInventory();
			if (CurrentInventory)
			{
				SetCurrentInventoryType(CurrentInventory);
				RefreshInventory(CurrentInventory);
				InventoryComp->OnInventoryChanged.AddDynamic(this, &UInventoryUI::OnInventoryChangedEvent);
			}
		}
	}
}

void UInventoryUI::OnWeaponBtnClick()
{
	if (InventoryComp)
	{
		UInventory* CurrentInventory = InventoryComp->GetWeaponInventory();
		SetCurrentInventoryType(CurrentInventory);
		RefreshInventory(CurrentInventory);
	}
}

void UInventoryUI::OnEquipmentBtnClick()
{
	if (InventoryComp)
	{
		UInventory* CurrentInventory = InventoryComp->GetEquipmentInventory();
		SetCurrentInventoryType(CurrentInventory);
		RefreshInventory(CurrentInventory);
	}
}

void UInventoryUI::OnConsumablesBtnClick()
{
	if (InventoryComp)
	{
		UInventory* CurrentInventory = InventoryComp->GetConsumablesInventory();
		SetCurrentInventoryType(CurrentInventory);
		RefreshInventory(CurrentInventory);
	}
}

void UInventoryUI::OnMaterialBtnClick()
{
	if (InventoryComp)
	{
		UInventory* CurrentInventory = InventoryComp->GetMaterialInventory();
		SetCurrentInventoryType(CurrentInventory);
		RefreshInventory(CurrentInventory);
	}
}

void UInventoryUI::OnTaskBtnClick()
{
	if (InventoryComp)
	{
		UInventory* CurrentInventory = InventoryComp->GetTaskItemInventory();
		SetCurrentInventoryType(CurrentInventory);
		RefreshInventory(CurrentInventory);
	}
}

void UInventoryUI::OnInventoryChangedEvent(UInventory* Inventory, int32 Index)
{
	if (Inventory->InventoryType != CurrentInventoryType)
	{
		return;
	}
	UItemSlotUI* ItemSlotUI = Cast<UItemSlotUI>(InventorySlot->GetChildAt(Index));
	if (ItemSlotUI)
	{
		SetItemSlotProperties(ItemSlotUI, Inventory, Index);
	}
}

void UInventoryUI::SetCurrentInventoryType(UInventory* Inventory)
{
	CurrentInventoryType = Inventory->InventoryType;
}

void UInventoryUI::RemoveWidget()
{
	InventorySlot->ClearChildren();
	InventoryComp->OnInventoryChanged.Remove(this, "OnInventoryChangedEvent");
	if (AQTPlayerController* PC = GetGamePlayerController())
	{
		PC->CurrentSelectedItemSlot = nullptr;
	}
	Super::RemoveWidget();
}

void UInventoryUI::RefreshInventory(UInventory* CurrentInventory)
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
		ItemSlot->InventoryComp = InventoryComp;
		if (ItemSlot)
		{
			InventorySlot->AddChildToUniformGrid(ItemSlot, i / ColumnNum, i % ColumnNum);
		}
	}
}

void UInventoryUI::SetItemSlotProperties(UItemSlotUI* ItemSlot, UInventory* Inventory, int32 Index)
{
	ItemSlot->SetItemIcon(Inventory->Inventory[Index].Item->ItemIcon);
	ItemSlot->SetItemAmount(Inventory->Inventory[Index].ItemAmounts);
	ItemSlot->SetItemType(Inventory->Inventory[Index].Item->GetItemType());
	ItemSlot->SetItemIndex(Index);
	ItemSlot->InventoryComp = InventoryComp;
	ItemSlot->InventoryObj = Inventory;
	ItemSlot->SlotType = EQTItemSlotType::EInventorySlot;
}
