// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "QTItem.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API UQTItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UQTItem()
		: bCanStacked(true)
	{
		
	}
	
	/** Type of this item, set in native parent class */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	/** User-visible short name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemName;

	/** User-visible long description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemDescription;
	
	/** Icon to display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	UTexture2D* ItemIcon;

	/** Price in game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 Price;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	bool bCanStacked;

	UFUNCTION(BlueprintCallable)
	FPrimaryAssetType GetItemType() const;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
