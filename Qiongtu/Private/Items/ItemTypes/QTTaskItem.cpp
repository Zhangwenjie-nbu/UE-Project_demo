// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemTypes/QTTaskItem.h"

#include "Core/GameCore/QTAssetManager.h"

UQTTaskItem::UQTTaskItem()
{
	ItemType = UQTAssetManager::TaskItemType;
	bCanStacked = true;
}
