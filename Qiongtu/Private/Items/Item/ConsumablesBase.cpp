// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item/ConsumablesBase.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AConsumablesBase::AConsumablesBase()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	PickUpSphereComp->SetupAttachment(RootComponent);
}
