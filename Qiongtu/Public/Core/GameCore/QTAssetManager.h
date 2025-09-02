// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "QTAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API UQTAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UQTAssetManager() {}

	static const FPrimaryAssetType WeaponItemType;
	static const FPrimaryAssetType EquipmentItemType;
	static const FPrimaryAssetType ConsumablesItemType;
	static const FPrimaryAssetType MaterialItemType;
	static const FPrimaryAssetType TaskItemType;
	static const FPrimaryAssetType EmptyItemType;
	
	static UQTAssetManager& Get();

	UQTItem* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);
};
