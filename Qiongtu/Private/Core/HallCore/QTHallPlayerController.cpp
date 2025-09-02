// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HallCore/QTHallPlayerController.h"
#include "UI/Core/UserWidgetBase.h"

AQTHallPlayerController::AQTHallPlayerController()
{
}

void AQTHallPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameAndUI());
	bShowMouseCursor = true;
	ChangeWidget(HallWidgetClass);
}

void AQTHallPlayerController::ESCPressed()
{
	if (CurrentWidget->CurrentWidget)
	{
		CurrentWidget->CurrentWidget->RemoveChildWidget();
	}
}

void AQTHallPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("ESC", EInputEvent::IE_Pressed, this, &AQTHallPlayerController::ESCPressed).bExecuteWhenPaused = true;
}

void AQTHallPlayerController::ChangeWidget(TSubclassOf<UUserWidgetBase> NewWidgetClass)
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveWidget();
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidgetBase>(GetWorld(), NewWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
