// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComp.h"
#include "Core/GameCore/QTAssetManager.h"
#include "Core/SubSystems/ArchiveSystem.h"
#include "Items/Item/ItemBase.h"
#include "Items/ItemTypes/QTEmptyItem.h"

// Sets default values for this component's properties

class UArchiveSystem;

UInventoryComp::UInventoryComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UInventoryComp::BeginPlay()
{
	Super::BeginPlay();
	if (UArchiveSystem* ArchiveSystem = GetOwner()->GetGameInstance()->GetSubsystem<UArchiveSystem>())
	{	
		ArchiveSystem->LoadInventoryToComp(this);
	}	
	if (EmptyItemID.IsValid())
	{
		InitInventory();
	}
}


bool UInventoryComp::AddItem(UInventory* InventoryToAdd, AItemBase* Item, UQTItem* ItemAsset)
{
	if (IsInventoryFull(InventoryToAdd))
	{
		return false;
	}
	int32 Index;
	if (ItemAsset->bCanStacked)
	{
		if (FindItemSlot(InventoryToAdd, Index, ItemAsset))
		{
			if (IsItemFull(InventoryToAdd, Index))
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("SlotIsFull"));
				return false;
			}
			if (MaxItemAmounts - InventoryToAdd->Inventory[Index].ItemAmounts < Item->ItemAmounts)
			{
				InventoryToAdd->Inventory[Index].ItemAmounts = MaxItemAmounts;
				Item->ItemAmounts -= MaxItemAmounts - InventoryToAdd->Inventory[Index].ItemAmounts;
				OnInventoryChanged.Broadcast(InventoryToAdd, Index);
				return true;
			}
			InventoryToAdd->Inventory[Index].ItemAmounts += Item->ItemAmounts;
			//全部拾取...
			Item->Destroy();
			OnInventoryChanged.Broadcast(InventoryToAdd, Index);
			return true;
		}
	}
	Index = FindEmptySlot(InventoryToAdd);
	FItemSlot ItemSlot;
	ItemSlot.Item = ItemAsset;
	ItemSlot.ItemAmounts = Item->ItemAmounts;
	ItemSlot.ItemData = Item->ItemAttributeData;
	InventoryToAdd->Inventory[Index] = ItemSlot;
	--InventoryToAdd->InvalidSlotAmounts;
	Item->Destroy();
	OnInventoryChanged.Broadcast(InventoryToAdd, Index);
	return true;
}

bool UInventoryComp::IsInventoryFull(UInventory* Inventory) const
{
	return Inventory->InvalidSlotAmounts <= 0;
}

bool UInventoryComp::IsItemFull(UInventory* Inventory, int32 _Index) const
{
	return Inventory->Inventory[_Index].ItemAmounts >= MaxItemAmounts;
}

bool UInventoryComp::FindItemSlot(UInventory* Inventory, int32& _Index, UQTItem* Item) const
{
	for (int i = 0; i < Inventory->Inventory.Num(); ++i)
	{
		if (Inventory->Inventory[i].Item->GetPrimaryAssetId() == Item->GetPrimaryAssetId())
		{
			_Index = i;
			return true;
		}
	}
	return false;
}


int32 UInventoryComp::FindEmptySlot(UInventory* Inventory) const
{
	for (int i = 0; i < Inventory->Inventory.Num(); ++i)
	{
		if ( Inventory->Inventory[i].Item->ItemType == UQTAssetManager::EmptyItemType)
		{
			return i;
		}
	}
	return 0;
}


// Called every frame
void UInventoryComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComp::InitInventory()
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
	for (int i = WeaponInventory->Inventory.Num(); i < MaxInventorySlot; ++i)
	{
		FItemSlot ItemSlot;
		ItemSlot.Item = EmptyItem;
		WeaponInventory->Inventory.Add(ItemSlot);
	}
	for (int i = EquipmentInventory->Inventory.Num(); i < MaxInventorySlot; ++i)
	{
		FItemSlot ItemSlot;
		ItemSlot.Item = EmptyItem;
		EquipmentInventory->Inventory.Add(ItemSlot);
	}
	for (int i = ConsumablesInventory->Inventory.Num(); i < MaxInventorySlot; ++i)
	{
		FItemSlot ItemSlot;
		ItemSlot.Item = EmptyItem;
		ConsumablesInventory->Inventory.Add(ItemSlot);
	}
	for (int i = MaterialInventory->Inventory.Num(); i < MaxInventorySlot; ++i)
	{
		FItemSlot ItemSlot;
		ItemSlot.Item = EmptyItem;
		MaterialInventory->Inventory.Add(ItemSlot);
	}
	for (int i = TaskItemInventory->Inventory.Num(); i < MaxInventorySlot; ++i)
	{
		FItemSlot ItemSlot;
		ItemSlot.Item = EmptyItem;
		TaskItemInventory->Inventory.Add(ItemSlot);
	}
}


bool UInventoryComp::AddItemToInventory(AItemBase* Item)
{
	if (Item == nullptr || Item->ItemAmounts == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Item == null"));
		return false;
	}
	UQTItem* ItemAsset = Cast<UQTItem>(UQTAssetManager::Get().GetPrimaryAssetObject(Item->ItemAssetID));
	if (ItemAsset == nullptr)
	{
		if (Item->ItemAssetID.IsValid())
		{
			ItemAsset = Cast<UQTItem>(UQTAssetManager::Get().ForceLoadItem(Item->ItemAssetID, true));
			if (ItemAsset == nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("AssetLoadFail"));
				return false;
			}
		}
		else return false;
	}
	FPrimaryAssetType& ItemAssetType = ItemAsset->ItemType;
	if (ItemAssetType == UQTAssetManager::WeaponItemType)
	{
		return AddItem(WeaponInventory, Item, ItemAsset);
	}
	else if (ItemAssetType == UQTAssetManager::EquipmentItemType)
	{
		return AddItem(EquipmentInventory, Item, ItemAsset);
	}
	else if (ItemAssetType == UQTAssetManager::ConsumablesItemType)
	{
		return AddItem(ConsumablesInventory, Item, ItemAsset);
	}
	else if (ItemAssetType == UQTAssetManager::MaterialItemType)
	{
		return AddItem(MaterialInventory, Item, ItemAsset);
	}
	else if (ItemAssetType == UQTAssetManager::TaskItemType)
	{
		return AddItem(TaskItemInventory, Item, ItemAsset);
	}
	return false;
}

bool UInventoryComp::DiscardItemInInventory(const FPrimaryAssetType& ItemType, int32 _Index, int32 Amount)
{
	if (ItemType == UQTAssetManager::EmptyItemType) return false;
	if (ItemType == UQTAssetManager::WeaponItemType)
	{
		return RemoveItem(WeaponInventory, _Index, Amount);
	}
	else if (ItemType == UQTAssetManager::EquipmentItemType)
	{
		return RemoveItem(EquipmentInventory, _Index, Amount);
	}
	else if (ItemType == UQTAssetManager::ConsumablesItemType)
	{
		return RemoveItem(ConsumablesInventory, _Index, Amount);
	}
	else if (ItemType == UQTAssetManager::MaterialItemType)
	{
		return RemoveItem(MaterialInventory, _Index, Amount);
	}
	else if (ItemType == UQTAssetManager::TaskItemType)
	{
		return RemoveItem(TaskItemInventory, _Index, Amount);
	}
	return false;
}

bool UInventoryComp::RemoveItem(UInventory* InventoryToRemoved, int32 _Index, int32 RemoveAmount)
{
	InventoryToRemoved->Inventory[_Index].ItemAmounts = FMath::Clamp(
								InventoryToRemoved->Inventory[_Index].ItemAmounts - RemoveAmount, 0, MaxItemAmounts);
	if (InventoryToRemoved->Inventory[_Index].ItemAmounts <= 0)
	{
		UQTEmptyItem* EmptyItem = Cast<UQTEmptyItem>(UQTAssetManager::Get().GetPrimaryAssetObject(EmptyItemID));
		if (EmptyItem)
		{
			FItemSlot ItemSlot;
			ItemSlot.Item = EmptyItem;
			InventoryToRemoved->Inventory[_Index] = ItemSlot;
			OnInventoryChanged.Broadcast(InventoryToRemoved, _Index);
			return true;
		}
		return false;
	}
	OnInventoryChanged.Broadcast(InventoryToRemoved, _Index);
	return true;
}

void UInventoryComp::SwapItem(const FPrimaryAssetType& ItemType1, const FPrimaryAssetType& ItemType2, int32 Index1, int32 Index2)
{
	if (ItemType1 == UQTAssetManager::EmptyItemType && ItemType2 == UQTAssetManager::EmptyItemType)
	{
		return;
	}
	if (Index1 >= MaxInventorySlot || Index2 >= MaxInventorySlot)
	{
		return;
	}
	FPrimaryAssetType ItemType;
	if (ItemType1 != UQTAssetManager::EmptyItemType)
	{
		ItemType = ItemType1;
	}
	else if (ItemType2 != UQTAssetManager::EmptyItemType)
	{
		ItemType = ItemType2;
	}
	FItemSlot TempItemSlot;
	if (ItemType == UQTAssetManager::WeaponItemType)
	{
		TempItemSlot = WeaponInventory->Inventory[Index1];
		WeaponInventory->Inventory[Index1] = WeaponInventory->Inventory[Index2];
		WeaponInventory->Inventory[Index2] = TempItemSlot;
		OnInventoryChanged.Broadcast(WeaponInventory, Index1);
		OnInventoryChanged.Broadcast(WeaponInventory, Index2);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("SwapSucceed"));
	}
	else if (ItemType == UQTAssetManager::EquipmentItemType)
	{
		TempItemSlot = EquipmentInventory->Inventory[Index1];
		EquipmentInventory->Inventory[Index1] = EquipmentInventory->Inventory[Index2];
		EquipmentInventory->Inventory[Index2] = TempItemSlot;
		OnInventoryChanged.Broadcast(EquipmentInventory, Index1);
		OnInventoryChanged.Broadcast(EquipmentInventory, Index2);
	}
	else if (ItemType == UQTAssetManager::ConsumablesItemType)
	{
		TempItemSlot = ConsumablesInventory->Inventory[Index1];
		ConsumablesInventory->Inventory[Index1] = ConsumablesInventory->Inventory[Index2];
		ConsumablesInventory->Inventory[Index2] = TempItemSlot;
		OnInventoryChanged.Broadcast(ConsumablesInventory, Index1);
		OnInventoryChanged.Broadcast(ConsumablesInventory, Index2);
	}
	else if (ItemType == UQTAssetManager::MaterialItemType)
	{
		TempItemSlot = MaterialInventory->Inventory[Index1];
		MaterialInventory->Inventory[Index1] = MaterialInventory->Inventory[Index2];
		MaterialInventory->Inventory[Index2] = TempItemSlot;
		OnInventoryChanged.Broadcast(MaterialInventory, Index1);
		OnInventoryChanged.Broadcast(MaterialInventory, Index2);
	}
	else if (ItemType == UQTAssetManager::TaskItemType)
	{
		TempItemSlot = TaskItemInventory->Inventory[Index1];
		TaskItemInventory->Inventory[Index1] = TaskItemInventory->Inventory[Index2];
		TaskItemInventory->Inventory[Index2] = TempItemSlot;
		OnInventoryChanged.Broadcast(TaskItemInventory, Index1);
		OnInventoryChanged.Broadcast(TaskItemInventory, Index2);
	}
}

UInventory* UInventoryComp::GetWeaponInventory()
{
	return WeaponInventory;
}

void UInventoryComp::SetWeaponInventory(UInventory* Inventory)
{
	WeaponInventory = Inventory;
}

void UInventoryComp::SetEquipmentInventory(UInventory* Inventory)
{
	EquipmentInventory = Inventory;
}

void UInventoryComp::SetConsumablesInventory(UInventory* Inventory)
{
	ConsumablesInventory = Inventory;
}

void UInventoryComp::SetMaterialInventory(UInventory* Inventory)
{
	MaterialInventory = Inventory;
}

void UInventoryComp::SetTaskItemInventory(UInventory* Inventory)
{
	TaskItemInventory = Inventory;
}

UInventory* UInventoryComp::GetEquipmentInventory()
{
	return EquipmentInventory;
}

UInventory* UInventoryComp::GetConsumablesInventory()
{
	return ConsumablesInventory;
}

UInventory* UInventoryComp::GetMaterialInventory()
{
	return MaterialInventory;
}

UInventory* UInventoryComp::GetTaskItemInventory()
{
	return TaskItemInventory;
}
