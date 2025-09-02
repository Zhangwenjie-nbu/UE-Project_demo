// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/QTMovementDelegates.h"
#include "Perception/AISense_Sight.h"
#include "QTAISense_Sight.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API UQTAISense_Sight : public UAISense_Sight
{
	GENERATED_BODY()
protected:
	virtual float Update() override;

public:
	FOnSightTargetsChanged OnSightTargetsChanged;
};
