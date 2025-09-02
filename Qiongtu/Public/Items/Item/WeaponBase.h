// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item/ItemBase.h"
#include "WeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()

public:
	AWeaponBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
	class USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
	class UBoxComponent* DefenseBoxComp;
	
};
