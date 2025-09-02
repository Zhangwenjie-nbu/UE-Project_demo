// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item/ItemBase.h"

#include "Character/Player/QTPlayerBase.h"
#include "Components/SphereComponent.h"
#include "Core/GameCore/QTAssetManager.h"

// Sets default values
AItemBase::AItemBase()
	: ItemAmounts(1)
	, bIsCloseToPlayer(false)
{
	PickUpSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpSphereComp"));
	PickUpSphereComp->SetCollisionProfileName(FName("PickUp_C"));
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	LoadAsset();
}

void AItemBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AQTPlayerBase* Player = Cast<AQTPlayerBase>(OtherActor);
	if (Player == nullptr)
	{
		return;
	}
	bIsCloseToPlayer = true;
}

void AItemBase::NotifyActorEndOverlap(AActor* OtherActor)
{
	AQTPlayerBase* Player = Cast<AQTPlayerBase>(OtherActor);
	if (Player == nullptr)
	{
		return;
	}
	bIsCloseToPlayer = false;
}

void AItemBase::LoadAsset()
{
	if (!ItemAssetID.IsValid())
	{
		return;
	}
	if (UQTAssetManager::Get().GetPrimaryAssetObject(ItemAssetID) == nullptr)
	{
		UQTAssetManager::Get().LoadPrimaryAsset(ItemAssetID);
	}
}



