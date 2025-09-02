// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item/TaskItemBase.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

ATaskItemBase::ATaskItemBase()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	PickUpSphereComp->SetupAttachment(RootComponent);
}
