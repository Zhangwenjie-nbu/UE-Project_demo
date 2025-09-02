// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class EQTMovementType : uint8
{
	ENone			UMETA(DisplayName = "None"),
	EGround			UMETA(DisplayName = "OnGround"),
	EFalling		UMETA(DisplayName = "Falling"),
	EAir			UMETA(DisplayName = "InAir"),
	EWater			UMETA(DisplayName = "InWater"),
};

UENUM(BlueprintType)
enum class EQTGait : uint8
{
	EWalk		  UMETA(DisplayName = "Walk"),
	ERun		  UMETA(DisplayName = "Run"),
	ESprint		  UMETA(DisplayName = "Sprint")
};

UENUM(BlueprintType)
enum class EQTStance : uint8
{
	EStance        UMETA(DisplayName = "Stance"),
	ECrouch		  UMETA(DisplayName = "Crouch")
};

UENUM(BlueprintType)
enum class EQTRotationMode : uint8
{
	EVelocityDirection        UMETA(DisplayName = "VelocityDirection"),
	ELookingDirection		  UMETA(DisplayName = "LookingDirection"),
	EAiming					  UMETA(DisplayName = "Aiming")
};

UENUM(BlueprintType)
enum class EQTMovementDirection : uint8
{
	EForward			UMETA(DisplayName = "Forward"),
	EBackWard			UMETA(DisplayName = "BackWard"),
	ELeft				UMETA(DisplayName = "Left"),
	ERight				UMETA(DisplayName = "Right"),
};

UENUM(BlueprintType)
enum class EQTHipsDirection : uint8
{
	EF			UMETA(DisplayName = "F"),
	EB			UMETA(DisplayName = "B"),
	ELF			UMETA(DisplayName = "LF"),
	ERF			UMETA(DisplayName = "RF"),
	ELB			UMETA(DisplayName = "LB"),
	ERB			UMETA(DisplayName = "RB")
};

UENUM(BlueprintType)
enum class EQTMovementAction : uint8
{
	ENone					UMETA(DisplayName = "None"),
	ELowMantle				UMETA(DisplayName = "LowMantle"),
	EHighMantle				UMETA(DisplayName = "HighMantle	"),
	EDodge					UMETA(DisplayName = "Dodge"),
	EGettingUp				UMETA(DisplayName = "GettingUp"),
};

UENUM(BlueprintType)
enum class EQTOverlayStates : uint8
{
	ENone				UMETA(DisplayName = "None"),
	EDefault			UMETA(DisplayName = "Default"),
	ESword				UMETA(DisplayName = "Sword	"),
	EGreatSword			UMETA(DisplayName = "GreatSword"),
	EInjured			UMETA(DisplayName = "Injured"),
	EHandsTied			UMETA(DisplayName = "HandsTied"),
	ETorch				UMETA(DisplayName = "Torch"),
	ERifle				UMETA(DisplayName = "Rifle"),
	EDefense			UMETA(DisplayName = "Defense")
};

UENUM()
enum class EQTCombatStates : uint8
{
	EIdle			UMETA(DisplayName = "Idle"),
	ECombat			UMETA(DisplayName = "Combat"),
};

UENUM(BlueprintType)
enum class EQTWeaponType : uint8
{
	ELightWeapon			UMETA(DisplayName = "LightWeapon"),
	EHeavyWeapon			UMETA(DisplayName = "HeavyWeapon")
};

UENUM(BlueprintType)
enum class EQTItemAttachPosition : uint8
{
	ELeftHand			UMETA(DisplayName = "LeftHand"),
	ERightHand			UMETA(DisplayName = "RightHand"),
	EBack				UMETA(DisplayName = "Back"),
};

UENUM(BlueprintType)
enum class EQTDodge : uint8
{
	EForwardDodge		UMETA(DisplayName = "ForwardDodge"),
	EBackDodge			UMETA(DisplayName = "BackDodge"),
	ELeftDodge			UMETA(DisplayName = "LeftDodge"),
	ERightDodge			UMETA(DisplayName = "RightDodge"),
};

UENUM(BlueprintType)
enum class EQTItemSlotType : uint8
{
	EInventorySlot		UMETA(DisplayName = "ForwardDodge"),
	EEquipmentSlot		UMETA(DisplayName = "BackDodge"),
};

UENUM(BlueprintType)
enum class EQTChuJueAnim : uint8
{
	ENone		UMETA(DisplayName = "None"),
	EAnSha		UMETA(DisplayName = "AnSha"),
};

UENUM(BlueprintType)
enum class EQTBaseState : uint8
{
	EIdle		UMETA(DisplayName = "Idle"),
	ECombat		UMETA(DisplayName = "Combat"),
};
