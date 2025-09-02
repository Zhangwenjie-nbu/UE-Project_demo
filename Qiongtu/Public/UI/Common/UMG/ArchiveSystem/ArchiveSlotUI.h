// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UserWidgetBase.h"
#include "ArchiveSlotUI.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API UArchiveSlotUI : public UUserWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	class UImage* ArchiveSlotImage;
	UFUNCTION()
	void OnArchiveSlotImageClickEvent();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SaveDate;

public:
	int16 SaveSlotIndex;
	void SetArchiveSlotImage(float Opacity);
	void SetSaveDate();
};
