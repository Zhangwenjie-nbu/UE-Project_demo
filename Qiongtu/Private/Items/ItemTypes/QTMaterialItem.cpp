// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemTypes/QTMaterialItem.h"

#include "Core/GameCore/QTAssetManager.h"

UQTMaterialItem::UQTMaterialItem()
{
	ItemType = UQTAssetManager::MaterialItemType;
	bCanStacked = true;
}
