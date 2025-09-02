// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item/EquipmentBase.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AEquipmentBase::AEquipmentBase()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	PickUpSphereComp->SetupAttachment(RootComponent);
}
