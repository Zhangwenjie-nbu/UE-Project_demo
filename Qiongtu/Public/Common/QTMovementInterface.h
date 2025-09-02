// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "QTStructs.h"
#include "UObject/Interface.h"
#include "QTMovementInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQTMovementInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class QIONGTU_API IQTMovementInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMovementValues(FMovementValues& _MovementValues);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMovementStates(FMovementStates& _MovementStates);
};
