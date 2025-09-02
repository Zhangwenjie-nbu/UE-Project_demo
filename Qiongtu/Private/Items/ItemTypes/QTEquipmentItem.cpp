// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemTypes/QTEquipmentItem.h"

#include "Core/GameCore/QTAssetManager.h"

UQTEquipmentItem::UQTEquipmentItem()
{
	ItemType = UQTAssetManager::EquipmentItemType;
	bCanStacked = false;
}
