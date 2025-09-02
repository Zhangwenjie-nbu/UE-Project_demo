// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UserWidgetBase.h"
#include "HallMainMenuUI.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API UHallMainMenuUI : public UUserWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	UPROPERTY(EditDefaultsOnly, Category = "WidgetAnimation")
	UWidgetAnimation* MenuWidgetAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "ChildWidgetClass")
	TSubclassOf<class UArchiveInterfaceUI> ArchiveInterfaceUIClass;
	
	UFUNCTION()
	void OnAnimationStartedEvent();
	UFUNCTION()
	void OnAnimationFinishedEvent();
	TArray<class UButton*> Buttons;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartGameButton;
	UFUNCTION()
	void StartGameBtnEvent();

	UPROPERTY(meta = (BindWidget))
	class UButton* LoadingButton;
	UFUNCTION()
	void LoadGameBtnEvent();

	UPROPERTY(meta = (BindWidget))
	class UButton* OptionButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitGameButton;
	UFUNCTION()
	void QuitGameBtnEvent();


public:
	virtual void RemoveWidget() override;
};
