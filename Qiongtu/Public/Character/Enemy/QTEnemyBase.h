// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameCore/QTCharacterBase.h"
#include "QTEnemyBase.generated.h"

/**
 * 
 */
class UQTWeaponItem;

UCLASS()
class QIONGTU_API AQTEnemyBase : public AQTCharacterBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly)
	UQTWeaponItem* CurrentWeaponItem;
	UPROPERTY(EditDefaultsOnly)
	FPrimaryAssetId CurrentWeaponId;

	virtual void BeginPlay() override;
public:
	virtual void InitWeapon();
};
