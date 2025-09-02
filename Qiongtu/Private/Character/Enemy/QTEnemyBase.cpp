// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/QTEnemyBase.h"

#include "Components/SphereComponent.h"
#include "Core/GameCore/QTAssetManager.h"
#include "Items/Item/WeaponBase.h"
#include "Items/ItemTypes/QTWeaponItem.h"

void AQTEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	InitWeapon();
}

void AQTEnemyBase::InitWeapon()
{
	CombatState = EQTCombatStates::ECombat;
	if (CurrentWeaponId.IsValid())
	{
		CurrentWeaponItem = Cast<UQTWeaponItem>(UQTAssetManager::Get().GetPrimaryAssetObject(CurrentWeaponId));
		if (CurrentWeaponItem)
		{
			FVector SpawnLocation = GetActorLocation();
			SpawnLocation.Z = SpawnLocation.Z + 200.f;
			SpawnLocation.X = SpawnLocation.X - 200.f;
			AWeaponBase* _CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(CurrentWeaponItem->WeaponClass, SpawnLocation, FRotator());
			SetCurrentWeapon(_CurrentWeapon);
		}
	}
}
