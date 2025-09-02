// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/QTComponentBase.h"
#include "Components/QTAbilitySystemComponent.h"
#include "Core/GameCore/QTCharacterBase.h"
#include "Core/GameCore/QTPlayerController.h"

// Sets default values for this component's properties
UQTComponentBase::UQTComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQTComponentBase::BeginPlay()
{
	Super::BeginPlay();
	MyCharacter = Cast<AQTPlayerController>(GetOwner())->MyPawn;
	if (MyCharacter.IsValid())
	{
		MyAbilitySystemComponent = Cast<UQTAbilitySystemComponent>(MyCharacter->GetAbilitySystemComponent());
	}
	
}


// Called every frame
void UQTComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

