// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item/ItemBase.h"
#include "ConsumablesBase.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API AConsumablesBase : public AItemBase
{
	GENERATED_BODY()
public:
	AConsumablesBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
	UStaticMeshComponent* MeshComp;
};
