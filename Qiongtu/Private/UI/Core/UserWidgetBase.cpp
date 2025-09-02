// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Core/UserWidgetBase.h"

#include "Core/GameCore/QTPlayerController.h"
#include "Core/HallCore/QTHallPlayerController.h"

void UUserWidgetBase::RemoveWidget()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveWidget();
	}
	this->RemoveFromViewport();
}

void UUserWidgetBase::RemoveChildWidget()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveChildWidget();
	}
	else
	{
		if (BaseClass)
		{
			BaseClass->ChangeWidget(nullptr);
		}
		else
		{
			if (GetGamePlayerController() != nullptr)
			{
				GetGamePlayerController()->ChangeWidget(nullptr);
			}
			else if (GetHallPlayerController() != nullptr)
			{
				GetHallPlayerController()->ChangeWidget(nullptr);
			}
		}
	}	
}

void UUserWidgetBase::ForwardPlayAnimation(UWidgetAnimation* WidgetAnim)
{
	if (!WidgetAnim)
	{
		return;
	}
	PlayAnimation(WidgetAnim);
}

void UUserWidgetBase::ReversePlayWidgetAnimation(UWidgetAnimation* WidgetAnim)
{
	if (!WidgetAnim)
	{
		return;
	}
	PlayAnimationReverse(WidgetAnim);
}

AQTHallPlayerController* UUserWidgetBase::GetHallPlayerController()
{
	return Cast<AQTHallPlayerController>(GetOwningPlayer());
}

AQTPlayerController* UUserWidgetBase::GetGamePlayerController()
{
	return Cast<AQTPlayerController>(GetOwningPlayer());
}

void UUserWidgetBase::ChangeWidget(TSubclassOf<UUserWidgetBase> NewWidgetClass)
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveWidget();
		CurrentWidget = nullptr;
		this->SetVisibility(ESlateVisibility::Visible);
	}
	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidgetBase>(GetWorld(), NewWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
			CurrentWidget->BaseClass = this;
			this->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
