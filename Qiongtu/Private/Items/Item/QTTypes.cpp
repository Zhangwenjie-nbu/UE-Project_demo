#include "Items/Item/QTTypes.h"

FInventoryData& FInventoryData::operator=(const UInventory* _Inventory)
{
	InvalidSlotAmounts = _Inventory->InvalidSlotAmounts;
	InventoryType = _Inventory->InventoryType;
	this->Inventory = _Inventory->Inventory;
	return *this;
}

void UInventory::SetInventoryFromInventoryData(FInventoryData InventoryData)
{
	this->InvalidSlotAmounts = InventoryData.InvalidSlotAmounts;
	this->InventoryType = InventoryData.InventoryType;
	this->Inventory = InventoryData.Inventory;
}
