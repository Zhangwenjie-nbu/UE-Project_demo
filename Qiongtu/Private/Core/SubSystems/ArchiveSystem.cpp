// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SubSystems/ArchiveSystem.h"

#include "Common/GameUserSettingsSaveGame.h"
#include "Common/QTBlueprintFunctionLibrary.h"
#include "Common/SaveGameDateArchive.h"
#include "Components/InventoryComp.h"
#include "Core/GameCore/QTAssetManager.h"
#include "Core/GameCore/QTGameInstance.h"
#include "Core/GameCore/QTSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UArchiveSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeInventory();
	InitializeDateTime();
	InitializeGameUserSettings();
	CurrentSaveSlotIndex = 0;
}

void UArchiveSystem::Deinitialize()
{
	Super::Deinitialize();
}

void UArchiveSystem::InitializeDateTime()
{
	SaveDate = {"", "", "", "", ""};
	if (UGameplayStatics::DoesSaveGameExist("ArchiveDateTime", 99))
	{
		USaveGameDateArchive* SaveGameDateArchive = Cast<USaveGameDateArchive>( UGameplayStatics::LoadGameFromSlot(
														"ArchiveDateTime", 99));
		if (SaveGameDateArchive)
		{
			SaveDate = SaveGameDateArchive->SaveGameDate;
		}
	}
}

void UArchiveSystem::InitializeInventory()
{
	WeaponInventory = NewObject<UInventory>();
	EquipmentInventory = NewObject<UInventory>();
	ConsumablesInventory = NewObject<UInventory>();
	MaterialInventory = NewObject<UInventory>();
	TaskItemInventory = NewObject<UInventory>();
	
	WeaponInventory->InventoryType = UQTAssetManager::WeaponItemType;
	EquipmentInventory->InventoryType = UQTAssetManager::EquipmentItemType;
	ConsumablesInventory->InventoryType = UQTAssetManager::ConsumablesItemType;
	MaterialInventory->InventoryType = UQTAssetManager::MaterialItemType;
	TaskItemInventory->InventoryType = UQTAssetManager::TaskItemType;
}


void UArchiveSystem::InitializeGameUserSettings()
{
	UGameUserSettingsSaveGame* GUSSaveGame = Cast<UGameUserSettingsSaveGame>(UGameplayStatics::LoadGameFromSlot(
									UGameUserSettingsSaveGame::SaveSlotName, UGameUserSettingsSaveGame::SaveSlotIndex));
	UQTGameInstance* GI = Cast<UQTGameInstance>(GetGameInstance());
	if (GUSSaveGame)
	{
		GI->UserSettings = GUSSaveGame->UserSettings;
	}
	else
	{
		GI->UserSettings.GlobalVolume = 0.5f;
		GI->UserSettings.BGMVolume = 0.5f;
		GI->UserSettings.UIVolume = 0.5f;
	}
}

void UArchiveSystem::SaveGameUserSettings()
{
	UGameUserSettingsSaveGame* GUSSaveGame = Cast<UGameUserSettingsSaveGame>(UGameplayStatics::CreateSaveGameObject(UGameUserSettingsSaveGame::StaticClass()));
	if (GUSSaveGame)
	{
		UQTGameInstance* GI = Cast<UQTGameInstance>(GetGameInstance());
		GUSSaveGame->UserSettings = GI->UserSettings;
		UGameplayStatics::AsyncSaveGameToSlot(GUSSaveGame, UGameUserSettingsSaveGame::SaveSlotName, UGameUserSettingsSaveGame::SaveSlotIndex);
	}
}


bool UArchiveSystem::FindEmptySaveSlot(int32& _SaveSlotIndex)
{
	for (int i = 0; i < 5; ++i)
	{
		if (!IsArchiveExisted(i))
		{
			_SaveSlotIndex = i;
			return true;
		}
	}
	return false;
}

void UArchiveSystem::SaveArchive()
{
	CurrentSaveSlotIndex = FMath::Clamp(CurrentSaveSlotIndex, 0, UQTSaveGame::SaveSlotIndex.Num());
	UQTSaveGame* SaveGameInstance = Cast<UQTSaveGame>(UGameplayStatics::CreateSaveGameObject(UQTSaveGame::StaticClass()));
	if (SaveGameInstance == nullptr)
	{
		return;
	}
	SaveInventoryToArchive(SaveGameInstance);
	SaveDateTimerToSlot();
	FAsyncSaveGameToSlotDelegate OnSaveGameToSlotCompleted;
	OnSaveGameToSlotCompleted.BindUObject(this, &UArchiveSystem::OnSaveGameToSlotCompletedEvent);
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, UQTSaveGame::SaveSlotName[CurrentSaveSlotIndex],
		UQTSaveGame::SaveSlotIndex[CurrentSaveSlotIndex], OnSaveGameToSlotCompleted);
}

bool UArchiveSystem::StartNewGame()
{
	int32 EmptySaveSlot;
	if (FindEmptySaveSlot(EmptySaveSlot))
	{
		CurrentSaveSlotIndex = EmptySaveSlot;
		UQTBlueprintFunctionLibrary::PlayLoadingScreen(GetWorld(), nullptr, false, 3.0f);
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Desert"));
		return true;
	}
	return false;
}

void UArchiveSystem::SaveInventoryToArchive(class UQTSaveGame* SaveGame)
{
	if (SaveGame)
	{
		SaveGame->WeaponInventory = WeaponInventory;
		SaveGame->EquipmentInventory = EquipmentInventory;
		SaveGame->ConsumablesInventory = ConsumablesInventory;
		SaveGame->MaterialInventory = MaterialInventory;
		SaveGame->TaskItemInventory = TaskItemInventory;
	}
}

void UArchiveSystem::OnSaveGameToSlotCompletedEvent(const FString& SaveGameName, const int32 SaveGameIndex,
	bool bSaveSucceed)
{
	if (bSaveSucceed)
	{
		//
		return;
	}
	//
}

void UArchiveSystem::SaveDateTimerToSlot()
{
	USaveGameDateArchive* SaveGameInstance = Cast<USaveGameDateArchive>(UGameplayStatics::CreateSaveGameObject(
																				USaveGameDateArchive::StaticClass()));
	SaveDate[CurrentSaveSlotIndex] = FDateTime::Now().ToString();
	SaveGameInstance->SaveGameDate = this->SaveDate;
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, "ArchiveDateTime",99);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, SaveGameInstance->SaveGameDate[CurrentSaveSlotIndex]);
}


void UArchiveSystem::LoadInventoryFromSlot(UQTSaveGame* SaveGame)
{
	WeaponInventory->SetInventoryFromInventoryData(SaveGame->WeaponInventory);
	EquipmentInventory->SetInventoryFromInventoryData(SaveGame->EquipmentInventory);
	ConsumablesInventory->SetInventoryFromInventoryData(SaveGame->ConsumablesInventory);
	MaterialInventory->SetInventoryFromInventoryData(SaveGame->MaterialInventory);
	TaskItemInventory->SetInventoryFromInventoryData(SaveGame->TaskItemInventory);
	//SaveGame->BeginDestroy();
}

bool UArchiveSystem::LoadInventoryToComp(UInventoryComp* _InventoryComp)
{
	if (_InventoryComp == nullptr)
	{
		return false;
	}
	_InventoryComp->SetWeaponInventory(WeaponInventory);
	_InventoryComp->SetEquipmentInventory(EquipmentInventory);
	_InventoryComp->SetConsumablesInventory(ConsumablesInventory);
	_InventoryComp->SetMaterialInventory(MaterialInventory);
	_InventoryComp->SetTaskItemInventory(TaskItemInventory);
	this->InventoryComp = _InventoryComp;
	return true;
}

bool UArchiveSystem::IsArchiveExisted(int32 ArchiveIndex)
{
	ArchiveIndex = FMath::Clamp(ArchiveIndex, 0, UQTSaveGame::SaveSlotName.Num());
	if (UGameplayStatics::DoesSaveGameExist(UQTSaveGame::SaveSlotName[ArchiveIndex], UQTSaveGame::SaveSlotIndex[ArchiveIndex]))
	{
		return true;
	}
	return false;
}

void UArchiveSystem::LoadArchive(int32 ArchiveIndex)
{
	ArchiveIndex = FMath::Clamp(ArchiveIndex, 0, 4);
	if (!IsArchiveExisted(ArchiveIndex))
	{
		return;
	}
	FAsyncLoadGameFromSlotDelegate OnLoadGameFromSlotCompleted;
	OnLoadGameFromSlotCompleted.BindUObject(this, &UArchiveSystem::OnLoadGameFromSlotCompletedEvent);
	UGameplayStatics::AsyncLoadGameFromSlot(UQTSaveGame::SaveSlotName[ArchiveIndex], UQTSaveGame::SaveSlotIndex[ArchiveIndex],
								OnLoadGameFromSlotCompleted);
}


void UArchiveSystem::OnLoadGameFromSlotCompletedEvent(const FString& SaveGameName, const int32 SaveGameIndex,
                                                      USaveGame* SaveGame)
{
	UQTSaveGame* CurrentSaveGame = Cast<UQTSaveGame>(SaveGame);
	if (CurrentSaveGame == nullptr)
	{
		return;
	}
	CurrentSaveSlotIndex = SaveGameIndex;
	LoadInventoryFromSlot(CurrentSaveGame);
}
