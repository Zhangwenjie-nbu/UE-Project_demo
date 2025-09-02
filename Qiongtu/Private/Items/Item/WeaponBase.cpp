// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item/WeaponBase.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

AWeaponBase::AWeaponBase()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	

	DefenseBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("DefenseComp"));
	DefenseBoxComp->SetupAttachment(RootComponent);
	

	PickUpSphereComp->SetupAttachment(RootComponent);
}
