// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AIControllers/Core/QTAIControllerBase.h"
#include "Character/Player/QTPlayerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void AQTAIControllerBase::SetLockSystem(AQTCharacterBase* Enemy)
{
	AQTCharacterBase* MyCharacter = Cast<AQTCharacterBase>(GetPawn());
	if (MyCharacter == nullptr)
	{
		return;
	}
	if (Enemy)
	{
		if (MyCharacter->MovementStates.RotationMode != EQTRotationMode::ELookingDirection)
		{
			MyCharacter->MovementStates.RotationMode = EQTRotationMode::ELookingDirection;
		}
		LockEnemy(Enemy);
	}
	else
	{
		if (MyCharacter->MovementStates.RotationMode != EQTRotationMode::EVelocityDirection)
		{
			MyCharacter->MovementStates.RotationMode = EQTRotationMode::EVelocityDirection;
		}
	}
}

void AQTAIControllerBase::LockEnemy(AQTCharacterBase* Enemy)
{
	if (Enemy == nullptr || GetPawn() == nullptr)
	{
		return;
	}
	FRotator _TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), Enemy->GetActorLocation());
	_TargetRotation.Roll = 0.f;
	FRotator TargetRotation = GetControlRotation();
	TargetRotation = FMath::RInterpConstantTo(TargetRotation, _TargetRotation,
					UGameplayStatics::GetWorldDeltaSeconds(this), 200.f);
	SetControlRotation(TargetRotation);
}

void AQTAIControllerBase::SetLockedSystemProperty(bool bLocked, AQTCharacterBase* TargetEnemy)
{
	bLockedEnemy = bLocked;
	CurrentEnemy = TargetEnemy;
}

void AQTAIControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SetLockSystem(CurrentEnemy);
}
