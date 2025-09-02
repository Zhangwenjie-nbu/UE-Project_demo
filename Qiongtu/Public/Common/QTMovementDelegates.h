// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QTStructs.h"
#include "UObject/NoExportTypes.h"
#include "QTMovementDelegates.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMovementStatesChanged, const FMovementStates&, NewMovementStates);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryChanged, UInventory*, Inventory, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShortcutChanged, FPrimaryAssetType, ItemType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSightTargetsChanged, AActor*, ChangedActor);


UCLASS()
class QIONGTU_API UQTMovementDelegates : public UObject
{
	GENERATED_BODY()
	
};

