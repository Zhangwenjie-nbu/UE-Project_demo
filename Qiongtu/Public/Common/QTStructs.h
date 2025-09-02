// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QTEnums.h"
#include "UObject/NoExportTypes.h"
#include "QTStructs.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FMovementValues
{
    GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "QTMovementValues")
	float Speed;
	UPROPERTY(BlueprintReadWrite, Category = "QTMovementValues")
	FVector Velocity;
	UPROPERTY(BlueprintReadWrite, Category = "QTMovementValues")
	FVector Acceleration;
	UPROPERTY(BlueprintReadWrite, Category = "QTMovementValues")
	FRotator AimingRotation;
	UPROPERTY(BlueprintReadWrite, Category = "QTMovementValues")
	FRotator CharacterRotation;
	UPROPERTY(BlueprintReadWrite, Category = "QTMovementValues")
	bool bJumped;
};

USTRUCT(BlueprintType)
struct FMovementStates
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "QTMovementState")
	EQTMovementType MovementType = EQTMovementType::EGround;
	
	UPROPERTY(BlueprintReadWrite, Category = "QTMovementState")
	EQTGait Gait = EQTGait::EWalk;

	UPROPERTY(BlueprintReadWrite, Category = "QTMovementState")
	EQTStance Stance = EQTStance::EStance;

	UPROPERTY(BlueprintReadWrite, Category = "QTMovementState")
	EQTRotationMode RotationMode = EQTRotationMode::EVelocityDirection;

	UPROPERTY(BlueprintReadWrite, Category = "QTMovementState")
	EQTMovementAction MovementAction = EQTMovementAction::ENone;

	UPROPERTY(BlueprintReadWrite, Category = "QTMovementState")
	EQTOverlayStates OverlayState = EQTOverlayStates::EDefault;
};

USTRUCT(BlueprintType)
struct FVelocityBlend
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	float F;
	UPROPERTY(BlueprintReadWrite)
	float B;
	UPROPERTY(BlueprintReadWrite)
	float L;
	UPROPERTY(BlueprintReadWrite)
	float R;
};

USTRUCT(BlueprintType)
struct FLeanAmount
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	float FB;
	UPROPERTY(BlueprintReadOnly)
	float LR;
};

