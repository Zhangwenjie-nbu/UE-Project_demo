// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item/QTTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ArchiveSystem.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API UArchiveSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	int32 CurrentSaveSlotIndex;
	UPROPERTY()
	TArray<FString> SaveDate;
	
protected:
	UPROPERTY()
	UInventory* WeaponInventory;
	UPROPERTY()
	UInventory* EquipmentInventory;
	UPROPERTY()
	UInventory* ConsumablesInventory;
	UPROPERTY()
	UInventory* MaterialInventory;
	UPROPERTY()
	UInventory* TaskItemInventory;

	UPROPERTY()
	class UInventoryComp* InventoryComp;
	
	UFUNCTION()
	void OnLoadGameFromSlotCompletedEvent(const FString& SaveGameName, const int32 SaveGameIndex, class USaveGame* SaveGame);

	UFUNCTION()
	void OnSaveGameToSlotCompletedEvent(const FString& SaveGameName, const int32 SaveGameIndex, bool bSaveSucceed);
	
	void SaveInventoryToArchive(class UQTSaveGame* SaveGame);
	void LoadInventoryFromSlot(class UQTSaveGame* SaveGame);
	void InitializeInventory();
	bool FindEmptySaveSlot(int32& _SaveSlotIndex);
	
	void InitializeDateTime();
	void SaveDateTimerToSlot();

	void InitializeGameUserSettings();

	
public:
	UFUNCTION()
	bool LoadInventoryToComp(class UInventoryComp* _InventoryComp);

	UFUNCTION()
	static bool IsArchiveExisted(int32 ArchiveIndex);

	UFUNCTION()
	void LoadArchive(int32 ArchiveIndex);

	UFUNCTION()
	void SaveArchive();

	UFUNCTION()
	bool StartNewGame();

	UFUNCTION(BlueprintCallable)
	void SaveGameUserSettings();
};


