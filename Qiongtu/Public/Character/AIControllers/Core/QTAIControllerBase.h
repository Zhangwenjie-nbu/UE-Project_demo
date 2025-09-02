// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Common/QTMovementDelegates.h"
#include "QTAIControllerBase.generated.h"

class UQTAISense_Sight;
/**
 * 
 */
UCLASS()
class QIONGTU_API AQTAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetLockSystem(class AQTCharacterBase* Enemy);
	
	UFUNCTION(BlueprintCallable)
	void LockEnemy(AQTCharacterBase* Enemy);

	UPROPERTY(BlueprintReadWrite)
	bool bLockedEnemy = false;

	UPROPERTY(BlueprintReadWrite)
	AQTCharacterBase* CurrentEnemy;

	UFUNCTION(BlueprintCallable)
	void SetLockedSystemProperty(bool bLocked, AQTCharacterBase* TargetEnemy);
	
	virtual void Tick(float DeltaSeconds) override;
};
