// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "QTGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API UQTGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = GASNotify)
	FName GASNotifyName;
};
