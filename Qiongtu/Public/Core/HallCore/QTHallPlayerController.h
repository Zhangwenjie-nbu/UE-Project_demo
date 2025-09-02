// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "QTHallPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API AQTHallPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AQTHallPlayerController();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UUserWidgetBase> HallWidgetClass;

	UPROPERTY()
	class UUserWidgetBase* CurrentWidget;

	void ESCPressed();

public:
	virtual void SetupInputComponent() override;
	void ChangeWidget(TSubclassOf<class UUserWidgetBase> NewWidgetClass);
};
