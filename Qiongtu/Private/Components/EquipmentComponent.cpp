// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EquipmentComponent.h"
#include "Core/GameCore/QTAssetManager.h"
#include "Core/GameCore/QTCharacterBase.h"
#include "Items/Item/WeaponBase.h"
#include "Items/ItemTypes/QTEmptyItem.h"
#include "Items/ItemTypes/QTWeaponItem.h"

bool UEquipmentComponent::SwapInventoryItem(UInventory* Inventory1, UInventory* Inventory2, int Index1, int Index2)
{
	if (Inventory1->Inventory.Num() < Index1 || Index2 > Inventory2->Inventory.Num())
	{
		return false;
	}
	const FItemSlot TempItemSlot = Inventory1->Inventory[Index1];
	Inventory1->Inventory[Index1] = Inventory2->Inventory[Index2];
	Inventory2->Inventory[Index2] = TempItemSlot;
	return true;
}

int32 UEquipmentComponent::GetNextValidIndex(UInventory* _Inventory, int32 CurrentIndex)
{
	int32 NextValidIndex = (++CurrentIndex) % _Inventory->Inventory.Num();
	for (int32 i = 0; i < _Inventory->Inventory.Num(); ++i)
	{
		if (_Inventory->Inventory[NextValidIndex].Item->ItemType != UQTAssetManager::EmptyItemType)
		{
			return NextValidIndex;
		}
		NextValidIndex = (++CurrentIndex) % _Inventory->Inventory.Num();
	}
	return NextValidIndex % _Inventory->Inventory.Num();
}

void UEquipmentComponent::SetCurrentWeapon()
{
	int32 TempIndex = CurrentWeaponIndex;
	CurrentWeaponIndex = GetNextValidIndex(WeaponInventory, CurrentWeaponIndex);
	OnShortCutChanged.Broadcast(UQTAssetManager::WeaponItemType);
	if (MyCharacter.IsValid())
	{
		if (Cast<UQTWeaponItem>(WeaponInventory->Inventory[CurrentWeaponIndex].Item))
		{
			MyCharacter->UpdateCurrentItem(WeaponInventory->Inventory[CurrentWeaponIndex]);
		}
	}
}

void UEquipmentComponent::SetCurrentConsumable()
{
	CurrentConsumableIndex = GetNextValidIndex(ConsumablesInventory, CurrentConsumableIndex);
	OnShortCutChanged.Broadcast(UQTAssetManager::ConsumablesItemType);
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	if (EmptyItemID.IsValid())
	{
		InitInventory();
	}
}

void UEquipmentComponent::InitInventory()
{
	UQTEmptyItem* EmptyItem = Cast<UQTEmptyItem>(UQTAssetManager::Get().GetPrimaryAssetObject(EmptyItemID));
	if (!IsValid(EmptyItem))
	{
		EmptyItem = Cast<UQTEmptyItem>(UQTAssetManager::Get().ForceLoadItem(EmptyItemID, true));
	}
	if (!IsValid(EmptyItem))
	{
		return;
	}
	WeaponInventory = NewObject<UInventory>();
	ArmorInventory = NewObject<UInventory>();
	ConsumablesInventory = NewObject<UInventory>();
	WeaponInventory->InventoryType = UQTAssetManager::WeaponItemType;
	ArmorInventory->InventoryType = UQTAssetManager::EquipmentItemType;
	ConsumablesInventory->InventoryType = UQTAssetManager::ConsumablesItemType;
	for (int i = WeaponInventory->Inventory.Num(); i < MaxWeaponSlot; ++i)
	{
		FItemSlot ItemSlot;
		ItemSlot.Item = EmptyItem;
		WeaponInventory->Inventory.Add(ItemSlot);
	}
	for (int i = ArmorInventory->Inventory.Num(); i < MaxArmorSlot; ++i)
	{
		FItemSlot ItemSlot;
		ItemSlot.Item = EmptyItem;
		ArmorInventory->Inventory.Add(ItemSlot);
	}
	for (int i = ConsumablesInventory->Inventory.Num(); i < MaxConsumablesSlot; ++i)
	{
		FItemSlot ItemSlot;
		ItemSlot.Item = EmptyItem;
		ConsumablesInventory->Inventory.Add(ItemSlot);
	}
}
