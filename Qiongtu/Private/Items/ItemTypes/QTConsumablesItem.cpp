// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemTypes/QTConsumablesItem.h"

#include "Core/GameCore/QTAssetManager.h"

UQTConsumablesItem::UQTConsumablesItem()
{
	ItemType = UQTAssetManager::ConsumablesItemType;
	bCanStacked = true;
}
