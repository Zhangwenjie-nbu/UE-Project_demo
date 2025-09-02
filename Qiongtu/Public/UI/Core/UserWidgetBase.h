// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class QIONGTU_API UUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void RemoveWidget();
	void RemoveChildWidget();
	void ForwardPlayAnimation(UWidgetAnimation* WidgetAnim);
	void ReversePlayWidgetAnimation(UWidgetAnimation* WidgetAnim);
	class AQTHallPlayerController* GetHallPlayerController();
	class AQTPlayerController* GetGamePlayerController();
	UPROPERTY()
	class UUserWidgetBase* CurrentWidget;
	
	void ChangeWidget(TSubclassOf<class UUserWidgetBase> NewWidgetClass);

	UPROPERTY()
	UUserWidgetBase* BaseClass;
};
