// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/QTMovementDelegates.h"
#include "Components/ActorComponent.h"
#include "Items/Item/QTTypes.h"
#include "InventoryComp.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QIONGTU_API UInventoryComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	UInventory* WeaponInventory;
	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	UInventory* EquipmentInventory;
	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	UInventory* ConsumablesInventory;
	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	UInventory* MaterialInventory;
	UPROPERTY(BlueprintReadWrite, Category = Inventory)
	UInventory* TaskItemInventory;

	UPROPERTY(EditDefaultsOnly)
	FPrimaryAssetId EmptyItemID;

	bool AddItem(UInventory* InventoryToAdd, class AItemBase* Item, UQTItem* ItemAsset);
	bool RemoveItem(UInventory* InventoryToRemoved, int32 _Index, int32 RemoveAmount);

	bool IsInventoryFull(UInventory* Inventory) const;
	bool IsItemFull(UInventory* Inventory, int32 _Index) const;
	bool FindItemSlot(UInventory* Inventory, int32& _Index, class UQTItem* Item) const;
	int32 FindEmptySlot(UInventory* Inventory) const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitInventory();

	UFUNCTION(BlueprintCallable)
	bool AddItemToInventory(AItemBase* Item);
	UFUNCTION(BlueprintCallable)
	bool DiscardItemInInventory(const FPrimaryAssetType& ItemType, int32 _Index, int32 Amount);
	UFUNCTION(BlueprintCallable)
	void SwapItem(const FPrimaryAssetType& ItemType1, const FPrimaryAssetType& ItemType2, int32 Index1, int32 Index2);

	UFUNCTION()
	UInventory* GetWeaponInventory();
	UFUNCTION()
	void SetWeaponInventory(UInventory* Inventory);
	
	UFUNCTION()
	UInventory* GetEquipmentInventory();
	UFUNCTION()
	void SetEquipmentInventory(UInventory* Inventory);
	
	UFUNCTION()
	UInventory* GetConsumablesInventory();
	UFUNCTION()
	void SetConsumablesInventory(UInventory* Inventory);
	
	UFUNCTION()
	UInventory* GetMaterialInventory();
	UFUNCTION()
	void SetMaterialInventory(UInventory* Inventory);
	
	UFUNCTION()
	UInventory* GetTaskItemInventory();
	UFUNCTION()
	void SetTaskItemInventory(UInventory* Inventory);

	FOnInventoryChanged OnInventoryChanged;
};
