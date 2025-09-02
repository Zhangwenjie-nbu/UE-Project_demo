// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/QTPlayerBase.h"

#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

AQTPlayerBase::AQTPlayerBase()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void AQTPlayerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AQTPlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AQTPlayerBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AQTPlayerBase::MoveRight);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AQTCharacterBase::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AQTCharacterBase::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AQTPlayerBase::QTCrouch);

	PlayerInputComponent->BindAction("SwitchGait", IE_Pressed, this, &AQTPlayerBase::SwitchGait);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AQTPlayerBase::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AQTPlayerBase::StopSprint);
}

void AQTPlayerBase::MoveForward(float Value)
{
	ForwardValue = Value;
	if (Controller && Value != 0)
	{
		FRotator Rotation = Controller->GetControlRotation();
		if (GetCharacterMovement()->IsFalling() || GetCharacterMovement()->IsMovingOnGround())
		{
			Rotation.Pitch = 0.0f;
			Rotation.Roll = 0.0f;
		}
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AQTPlayerBase::MoveRight(float Value)
{
	RightValue = Value;
	if (Controller && Value != 0)
	{
		FRotator Rotation = Controller->GetControlRotation();
		if (GetCharacterMovement()->IsFalling() || GetCharacterMovement()->IsMovingOnGround())
		{
			Rotation.Pitch = 0.0f;
			Rotation.Roll = 0.0f;
		}
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AQTPlayerBase::QTCrouch()
{
	if (MovementStates.Stance == EQTStance::EStance)
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}


void AQTPlayerBase::SwitchGait()
{
	if (DesireGait == EQTGait::EWalk)
	{
		DesireGait = EQTGait::ERun;
		DesireGaitChanged();
	}
	else if (DesireGait == EQTGait::ERun)
	{
		DesireGait = EQTGait::EWalk;
		DesireGaitChanged();
	}
}

void AQTPlayerBase::StartSprint()
{
	DesireGait = EQTGait::ESprint;
	GetWorldTimerManager().SetTimer(SprintTimerHandle, this, &AQTPlayerBase::SprintChecked, 0.1f, true);
}

void AQTPlayerBase::StopSprint()
{
	DesireGait = EQTGait::ERun;
	GetWorldTimerManager().ClearTimer(SprintTimerHandle);
	DesireGaitChanged();
}


void AQTPlayerBase::SprintChecked()
{
	if (MovementStates.Gait != EQTGait::ESprint)
	{
		DesireGaitChanged();	
	}
}

EQTDodge AQTPlayerBase::GetCurrentDodgeDirection()
{
	if (RightValue != 0)
	{
		return CurrentDodgeDirection = RightValue == 1 ? EQTDodge::ERightDodge : EQTDodge::ELeftDodge;
	}
	else if (ForwardValue != 0)
	{
		return CurrentDodgeDirection = ForwardValue == 1 ? EQTDodge::EForwardDodge : EQTDodge::EBackDodge;
	}
	else
	{
		return CurrentDodgeDirection = EQTDodge::EForwardDodge;
	}
}
