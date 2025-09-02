// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item/ItemBase.h"
#include "EquipmentBase.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API AEquipmentBase : public AItemBase
{
	GENERATED_BODY()
public:
	AEquipmentBase();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
	UStaticMeshComponent* MeshComp;
};
