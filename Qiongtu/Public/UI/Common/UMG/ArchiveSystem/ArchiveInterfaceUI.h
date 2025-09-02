// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UserWidgetBase.h"
#include "ArchiveInterfaceUI.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API UArchiveInterfaceUI : public UUserWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

	void SetArchiveSlotVisibility();
	void SetArchiveSlotProperties();

	UPROPERTY(meta = (BindWidget))
	class UArchiveSlotUI* FirstArchiveSlot;
	
	UPROPERTY(meta = (BindWidget))
	class UArchiveSlotUI* SecondArchiveSlot;
	
	UPROPERTY(meta = (BindWidget))
	class UArchiveSlotUI* ThirdArchiveSlot;
	
	UPROPERTY(meta = (BindWidget))
	class UArchiveSlotUI* ForthArchiveSlot;
	
	UPROPERTY(meta = (BindWidget))
	class UArchiveSlotUI* FifthArchiveSlot;

	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnBtn;
	UFUNCTION()
	void OnReturnBtnClickedEvent();

public:
	virtual void RemoveWidget() override;
};
