// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemTypes/QTEmptyItem.h"

#include "Core/GameCore/QTAssetManager.h"

UQTEmptyItem::UQTEmptyItem()
{
	ItemType = UQTAssetManager::EmptyItemType;
	bCanStacked = false;
	ItemName = FText::FromString("EmptyItem");
}
