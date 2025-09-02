// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/QTEnums.h"
#include "Items/Item/QTTypes.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UCLASS()
class QIONGTU_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemAttribute)
	TMap<EItemAttributeType, FItemAttributeData> ItemAttributeData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
	FPrimaryAssetId ItemAssetID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 ItemAmounts;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
	class USphereComponent* PickUpSphereComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
	EQTOverlayStates ChangeOverlayStateItemType = EQTOverlayStates::ENone;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
	EQTWeaponType WeaponType = EQTWeaponType::ELightWeapon;

	bool bIsCloseToPlayer;

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void LoadAsset();
};
