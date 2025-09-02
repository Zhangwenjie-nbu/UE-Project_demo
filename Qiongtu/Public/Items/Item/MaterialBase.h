// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item/ItemBase.h"
#include "MaterialBase.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API AMaterialBase : public AItemBase
{
	GENERATED_BODY()
public:
	AMaterialBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
	UStaticMeshComponent* MeshComp;
};
