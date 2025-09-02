// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Items/Item/QTTypes.h"
#include "GameUserSettingsSaveGame.generated.h"


/**
 * 
 */
UCLASS()
class QIONGTU_API UGameUserSettingsSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UGameUserSettingsSaveGame();
	static FString SaveSlotName;
	static int32 SaveSlotIndex;

	UPROPERTY()
	FQTUserSettings UserSettings;
};
