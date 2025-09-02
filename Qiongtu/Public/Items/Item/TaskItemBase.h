// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item/ItemBase.h"
#include "TaskItemBase.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API ATaskItemBase : public AItemBase
{
	GENERATED_BODY()
public:
	ATaskItemBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
	UStaticMeshComponent* MeshComp;
};
