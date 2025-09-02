// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemTypes/QTWeaponItem.h"
#include "Core/GameCore/QTAssetManager.h"

UQTWeaponItem::UQTWeaponItem()
{
	ItemType = UQTAssetManager::WeaponItemType;
	bCanStacked = false;
}
