// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameDateArchive.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API USaveGameDateArchive : public USaveGame
{
	GENERATED_BODY()
public:
	USaveGameDateArchive();
	UPROPERTY()
	TArray<FString> SaveGameDate;
};
