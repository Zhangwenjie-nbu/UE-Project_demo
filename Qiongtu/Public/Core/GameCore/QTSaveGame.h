// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "QTSaveGame.generated.h"

/**
 * 
 */


UCLASS()
class QIONGTU_API UQTSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UQTSaveGame();
	
	static TArray<FString> SaveSlotName;
	static TArray<int32> SaveSlotIndex;

	UPROPERTY()
	FInventoryData WeaponInventory;
	UPROPERTY()
	FInventoryData EquipmentInventory;
	UPROPERTY()
	FInventoryData ConsumablesInventory;
	UPROPERTY()
	FInventoryData MaterialInventory;
	UPROPERTY()
	FInventoryData TaskItemInventory;
};
