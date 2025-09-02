// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/EngineTypes.h"
#include "UObject/Object.h"
#include "QTTypes.generated.h"

/**
 * 
 */

const EObjectTypeQuery PickUpObjectTypeQuery = EObjectTypeQuery::ObjectTypeQuery7;

UENUM(BlueprintType)
enum class EItemAttributeType : uint8
{
	EAttackpower
};

USTRUCT(BlueprintType)
struct FItemAttributeData
{
	GENERATED_BODY()
public:
	FItemAttributeData()
		: MinValue(0.0f)
		, MaxValue(100.0f)
		, CurrentValue(100.0f)
	{}
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemAttributeData)
	float MinValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemAttributeData)
	float MaxValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemAttributeData)
	float CurrentValue;
};

const int32 MaxInventorySlot = 50;
const int32 MaxItemAmounts = 100;


USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = Item)
	class UQTItem* Item = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = Item)
	int32 ItemAmounts = 0;
	UPROPERTY(BlueprintReadOnly, Category = Item)
	TMap<EItemAttributeType, FItemAttributeData> ItemData;

	FItemSlot& operator = (const FItemSlot& OtherItemSlot)
	{
		this->Item = OtherItemSlot.Item;
		this->ItemAmounts = OtherItemSlot.ItemAmounts;
		this->ItemData = OtherItemSlot.ItemData;
		return *this;
	}
};

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()
public:
	FInventoryData()
		: InvalidSlotAmounts(MaxInventorySlot)
	{
		
	}
	int32 InvalidSlotAmounts;
	FPrimaryAssetType InventoryType;
	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	TArray<FItemSlot> Inventory;

	FInventoryData& operator = (const class UInventory* _Inventory);
};



UCLASS()
class UInventory : public UObject
{
	GENERATED_BODY()
public:
	UInventory()
		: InvalidSlotAmounts(MaxInventorySlot)
	{
		
	}
	int32 InvalidSlotAmounts;
	FPrimaryAssetType InventoryType;
	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	TArray<FItemSlot> Inventory;
	
	void SetInventoryFromInventoryData(FInventoryData InventoryData);
};


UENUM(BlueprintType)
enum class EQTLanguages : uint8 
{
	EDefault	UMETA(DisplayName = "Default"),
	ESimpleChinese UMETA(DisplayName = "简体中文"),
	EEnglish UMETA(DisplayName = "English")
};

USTRUCT(BlueprintType)
struct FQTUserSettings
{
	GENERATED_BODY()
public:
	FQTUserSettings()
		: GlobalVolume(0.5f)
		, BGMVolume(0.5f)
		, UIVolume(0.5f)
		, QTLanguage(EQTLanguages::ESimpleChinese)
	{}
	
	UPROPERTY(BlueprintReadWrite, Category = "UserSettings")
    float GlobalVolume;
    UPROPERTY(BlueprintReadWrite, Category = "UserSettings")
    float BGMVolume;
	UPROPERTY(BlueprintReadWrite, Category = "UserSettings")
	float UIVolume;
	UPROPERTY(BlueprintReadWrite, Category = "UserSettings")
	int32 CurrentResolutionIndex = 1;
	UPROPERTY(BlueprintReadWrite, Category = "UserSettings")
	int32 CurrentScreenIndex = 1;
	UPROPERTY(BlueprintReadWrite, Category = "UserSettings")
	int32 CurrentMaterialIndex = 1;
	UPROPERTY(BlueprintReadWrite, Category = "UserSettings")
	EQTLanguages QTLanguage;
};
