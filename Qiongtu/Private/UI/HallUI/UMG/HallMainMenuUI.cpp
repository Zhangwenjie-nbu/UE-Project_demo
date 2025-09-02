// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HallUI/UMG/HallMainMenuUI.h"
#include "Components/Button.h"
#include "Core/HallCore/QTHallPlayerController.h"
#include "Core/SubSystems/ArchiveSystem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Common/UMG/ArchiveSystem/ArchiveInterfaceUI.h"

void UHallMainMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Buttons.Add(StartGameButton);
	Buttons.Add(LoadingButton);
	Buttons.Add(OptionButton);
	Buttons.Add(QuitGameButton);
	
	MenuWidgetAnimation = GetWidgetTreeOwningClass()->Animations[0];
	if (MenuWidgetAnimation)
	{
		FWidgetAnimationDynamicEvent OnAnimationStarted;
		OnAnimationStarted.BindUFunction(this, "OnAnimationStartedEvent");
		BindToAnimationStarted(MenuWidgetAnimation, OnAnimationStarted);

		FWidgetAnimationDynamicEvent OnAnimationFinished;
		OnAnimationFinished.BindUFunction(this, "OnAnimationFinishedEvent");
		BindToAnimationFinished(MenuWidgetAnimation, OnAnimationFinished);

		ForwardPlayAnimation(MenuWidgetAnimation);
	}
	StartGameButton->OnClicked.AddDynamic(this, &UHallMainMenuUI::StartGameBtnEvent);
	LoadingButton->OnClicked.AddDynamic(this, &UHallMainMenuUI::LoadGameBtnEvent);
	QuitGameButton->OnClicked.AddDynamic(this, &UHallMainMenuUI::QuitGameBtnEvent);
}

void UHallMainMenuUI::OnAnimationStartedEvent()
{
	if (Buttons.Num() <= 0)
	{
		return;
	}
	for (UButton* button : Buttons)
	{
		button->SetIsEnabled(false);
	}
}

void UHallMainMenuUI::OnAnimationFinishedEvent()
{
	if (Buttons.Num() <= 0)
	{
		return;
	}
	for (UButton* button : Buttons)
	{
		button->SetIsEnabled(true);
	}
}

void UHallMainMenuUI::StartGameBtnEvent()
{
	AQTHallPlayerController* Controller = GetHallPlayerController();
	if (!Controller)
	{
		return;
	}
	UArchiveSystem* ArchiveSystem = GetGameInstance()->GetSubsystem<UArchiveSystem>();
	if (ArchiveSystem)
	{
		ArchiveSystem->StartNewGame();
		Controller->ChangeWidget(nullptr);	
	}
}

void UHallMainMenuUI::LoadGameBtnEvent()
{
	if (ArchiveInterfaceUIClass)
	{
		ChangeWidget(ArchiveInterfaceUIClass);
	}
}

void UHallMainMenuUI::QuitGameBtnEvent()
{
	AQTHallPlayerController* PC = GetHallPlayerController();
	if (PC)
	{
		PC->ConsoleCommand("quit");
	}
}

void UHallMainMenuUI::RemoveWidget()
{
	Super::RemoveWidget();
}
