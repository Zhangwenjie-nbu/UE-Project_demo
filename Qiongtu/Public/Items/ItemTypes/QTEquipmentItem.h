// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemTypes/QTItem.h"
#include "QTEquipmentItem.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API UQTEquipmentItem : public UQTItem
{
	GENERATED_BODY()
public:
	UQTEquipmentItem();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Item)
	TSubclassOf<class AEquipmentBase> EquipmentClass;
};
