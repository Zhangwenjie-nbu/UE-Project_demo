// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/QTAssetManager.h"

#include "Items/ItemTypes/QTItem.h"

const FPrimaryAssetType UQTAssetManager::WeaponItemType = TEXT("Weapon");
const FPrimaryAssetType UQTAssetManager::EquipmentItemType = TEXT("Equipment");
const FPrimaryAssetType UQTAssetManager::ConsumablesItemType = TEXT("Consumables");
const FPrimaryAssetType UQTAssetManager::MaterialItemType = TEXT("MaterialItem");
const FPrimaryAssetType UQTAssetManager::TaskItemType = TEXT("TaskItem");
const FPrimaryAssetType UQTAssetManager::EmptyItemType = TEXT("EmptyItem");

UQTAssetManager& UQTAssetManager::Get()
{
	UQTAssetManager* This = Cast<UQTAssetManager>(GEngine->AssetManager);
	if (This)
	{
		return *This;
	}
	return *NewObject<UQTAssetManager>();
}

UQTItem* UQTAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	UQTItem* LoadedItem = Cast<UQTItem>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}
