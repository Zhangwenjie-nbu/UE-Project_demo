// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemTypes/QTItem.h"
#include "QTConsumablesItem.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API UQTConsumablesItem : public UQTItem
{
	GENERATED_BODY()
public:
	UQTConsumablesItem();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Item)
	TSubclassOf<class AConsumablesBase> ConsumablesClass;
};
