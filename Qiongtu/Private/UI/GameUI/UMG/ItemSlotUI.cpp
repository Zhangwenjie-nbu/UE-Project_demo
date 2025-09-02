// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameUI/UMG/ItemSlotUI.h"

#include "Components/EquipmentComponent.h"
#include "Components/Image.h"
#include "Components/InventoryComp.h"
#include "Components/TextBlock.h"
#include "Core/GameCore/QTAssetManager.h"
#include "Core/GameCore/QTPlayerController.h"
#include "UI/GameUI/UMG/EquipmentUI.h"
#include "UI/GameUI/UMG/InventoryUI.h"

void UItemSlotUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}


void UItemSlotUI::SetItemIcon(UTexture2D* NewItemIcon)
{
	ItemIcon->SetBrushFromTexture(NewItemIcon);
	ItemIconTexture = NewItemIcon;
}

void UItemSlotUI::SetItemAmount(int32 _ItemAmount)
{
	ItemAmounts = _ItemAmount;
	if (_ItemAmount <= 1)
	{
		ItemAmountText->SetText(FText::FromString(""));
	}
	else
		ItemAmountText->SetText(FText::FromString(FString::FromInt(_ItemAmount)));
}

void UItemSlotUI::SetItemType(FPrimaryAssetType _ItemType)
{
	ItemType = _ItemType;
}

const FPrimaryAssetType& UItemSlotUI::GetItemType() const
{
	return ItemType;
}

bool UItemSlotUI::bCanDrag()
{
	if (ItemType == UQTAssetManager::EmptyItemType)
	{
		return false;
	}
	return true;
}

void UItemSlotUI::SetItemIndex(int32 _ItemIndex)
{
	ItemIndex = _ItemIndex;
}

void UItemSlotUI::SwapItemSlot(UItemSlotUI* ItemSlot1, UItemSlotUI* ItemSlot2)
{
	if (InventoryComp)
	{
		InventoryComp->SwapItem(ItemSlot1->ItemType, ItemSlot2->ItemType,
			ItemSlot1->ItemIndex, ItemSlot2->ItemIndex);
	}
}

void UItemSlotUI::SwapItem(UItemSlotUI* ItemSlot1, UItemSlotUI* ItemSlot2)
{
	if (ItemSlot1->InventoryObj->InventoryType != ItemSlot2->InventoryObj->InventoryType)
	{
		return;
	}
	if (ItemSlot1->SlotType == EQTItemSlotType::EInventorySlot)
	{
		SwapItemSlot(ItemSlot1, ItemSlot2);
		return;
	}
	const FItemSlot TempItemSlot = ItemSlot1->InventoryObj->Inventory[ItemSlot1->ItemIndex];
	ItemSlot1->InventoryObj->Inventory[ItemSlot1->ItemIndex] = ItemSlot2->InventoryObj->Inventory[ItemSlot2->ItemIndex];
	ItemSlot2->InventoryObj->Inventory[ItemSlot2->ItemIndex] = TempItemSlot;
	if (AQTPlayerController* PC = GetGamePlayerController())
	{
		PC->GetInventoryComp()->OnInventoryChanged.Broadcast(ItemSlot2->InventoryObj, ItemSlot2->ItemIndex);
		PC->GetEquipmentComp()->OnInventoryChanged.Broadcast(ItemSlot1->InventoryObj, ItemSlot1->ItemIndex);
	}
	if (ItemSlot1->SlotType == EQTItemSlotType::EEquipmentSlot)
	{
		if (AQTPlayerController* PC = GetGamePlayerController())
		{
			PC->ESCPressed();
		}
	}
}

void UItemSlotUI::SetCurrentSelectedItemSlot()
{
	if (AQTPlayerController* PC = GetGamePlayerController())
	{
		if (SlotType == EQTItemSlotType::EEquipmentSlot)
		{
			PC->CurrentSelectedItemSlot = this;
			if (EquipmentUI && InventoryUIClass)
			{
				EquipmentUI->ChangeWidget(InventoryUIClass);
			}
		}
		else
		{
			if (PC->CurrentSelectedItemSlot == nullptr)
			{
				PC->CurrentSelectedItemSlot = this;
			}
			else
			{
				SwapItem(PC->CurrentSelectedItemSlot, this);
				PC->CurrentSelectedItemSlot = nullptr;
			}
		}
	}
}

void UItemSlotUI::OnLeftMouseButton()
{
	SetCurrentSelectedItemSlot();
	
}
