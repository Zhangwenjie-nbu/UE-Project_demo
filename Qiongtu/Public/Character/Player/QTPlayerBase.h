// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameCore/QTCharacterBase.h"
#include "QTPlayerBase.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API AQTPlayerBase : public AQTCharacterBase
{
	GENERATED_BODY()
public:
	AQTPlayerBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UCameraComponent* CameraComp;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FORCEINLINE class UCameraComponent* GetCameraComp() { return CameraComp; }

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void QTCrouch();
	void SwitchGait();
	void StartSprint();
	void StopSprint();

//冲刺检测
	FTimerHandle SprintTimerHandle;
	void SprintChecked();

//翻滚方向
	virtual EQTDodge GetCurrentDodgeDirection() override;

};
