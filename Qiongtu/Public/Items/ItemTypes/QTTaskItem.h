// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemTypes/QTItem.h"
#include "QTTaskItem.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API UQTTaskItem : public UQTItem
{
	GENERATED_BODY()
public:
	UQTTaskItem();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Item)
	TSubclassOf<class ATaskItemBase> TaskItemClass;
};
