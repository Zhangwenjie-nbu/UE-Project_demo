// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "QTAbilitySystemComponent.generated.h"

/**
 * 
 */
class UQTGameplayAbility;
UCLASS()
class QIONGTU_API UQTAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UQTGameplayAbility*>& ActiveAbilities);
};
