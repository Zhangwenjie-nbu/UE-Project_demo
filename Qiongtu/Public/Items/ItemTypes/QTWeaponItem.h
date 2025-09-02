// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemTypes/QTItem.h"
#include "QTWeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API UQTWeaponItem : public UQTItem
{
	GENERATED_BODY()

public:
	UQTWeaponItem();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Item)
	TSubclassOf<class AWeaponBase> WeaponClass;
};
