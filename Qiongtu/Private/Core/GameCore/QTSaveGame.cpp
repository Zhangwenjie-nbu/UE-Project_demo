// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/QTSaveGame.h"

TArray<FString> UQTSaveGame::SaveSlotName = {"FirstArchive", "SecondArchive", "ThirdArchive", "ForthArchive", "FifthArchive"};
TArray<int32> UQTSaveGame::SaveSlotIndex = {0, 1, 2, 3, 4};
UQTSaveGame::UQTSaveGame()
{
}
