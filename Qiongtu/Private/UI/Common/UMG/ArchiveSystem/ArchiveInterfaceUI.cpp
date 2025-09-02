// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/UMG/ArchiveSystem/ArchiveInterfaceUI.h"

#include "Components/Button.h"
#include "Core/SubSystems/ArchiveSystem.h"
#include "UI/Common/UMG/ArchiveSystem/ArchiveSlotUI.h"

void UArchiveInterfaceUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetArchiveSlotVisibility();
	SetArchiveSlotProperties();

	ReturnBtn->OnClicked.AddDynamic(this, &UArchiveInterfaceUI::OnReturnBtnClickedEvent);
}

void UArchiveInterfaceUI::SetArchiveSlotVisibility()
{
	FirstArchiveSlot->SetVisibility(ESlateVisibility::Visible);
	SecondArchiveSlot->SetVisibility(ESlateVisibility::Visible);
	ThirdArchiveSlot->SetVisibility(ESlateVisibility::Visible);
	ForthArchiveSlot->SetVisibility(ESlateVisibility::Visible);
	FifthArchiveSlot->SetVisibility(ESlateVisibility::Visible);
	if (!UArchiveSystem::IsArchiveExisted(0))
	{
		FirstArchiveSlot->SetVisibility(ESlateVisibility::HitTestInvisible);
		FirstArchiveSlot->SetArchiveSlotImage(0.0f);
	}
	if (!UArchiveSystem::IsArchiveExisted(1))
	{
		SecondArchiveSlot->SetVisibility(ESlateVisibility::HitTestInvisible);
		SecondArchiveSlot->SetArchiveSlotImage(0.0f);
	}
	if (!UArchiveSystem::IsArchiveExisted(2))
	{
		ThirdArchiveSlot->SetVisibility(ESlateVisibility::HitTestInvisible);
		ThirdArchiveSlot->SetArchiveSlotImage(0.0f);
	}
	if (!UArchiveSystem::IsArchiveExisted(3))
	{
		ForthArchiveSlot->SetVisibility(ESlateVisibility::HitTestInvisible);
		ForthArchiveSlot->SetArchiveSlotImage(0.0f);
	}
	if (!UArchiveSystem::IsArchiveExisted(4))
	{
		FifthArchiveSlot->SetVisibility(ESlateVisibility::HitTestInvisible);
		FifthArchiveSlot->SetArchiveSlotImage(0.0f);
	}
}

void UArchiveInterfaceUI::SetArchiveSlotProperties()
{
	FirstArchiveSlot->SaveSlotIndex = 0;
	SecondArchiveSlot->SaveSlotIndex = 1;
	ThirdArchiveSlot->SaveSlotIndex = 2;
	ForthArchiveSlot->SaveSlotIndex = 3;
	FifthArchiveSlot->SaveSlotIndex = 4;
	FirstArchiveSlot->SetSaveDate();
	SecondArchiveSlot->SetSaveDate();
	ThirdArchiveSlot->SetSaveDate();
	ForthArchiveSlot->SetSaveDate();
	FifthArchiveSlot->SetSaveDate();
}

void UArchiveInterfaceUI::OnReturnBtnClickedEvent()
{
	if (BaseClass)
	{
		BaseClass->ChangeWidget(nullptr);
	}
}

void UArchiveInterfaceUI::RemoveWidget()
{
	RemoveFromViewport();
}
