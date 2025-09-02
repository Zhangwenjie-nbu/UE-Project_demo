// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemTypes/QTItem.h"

FPrimaryAssetType UQTItem::GetItemType() const
{
	return ItemType;
}

FPrimaryAssetId UQTItem::GetPrimaryAssetId() const
{
	
	return FPrimaryAssetId(ItemType, GetFName());
}
