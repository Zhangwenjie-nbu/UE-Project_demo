// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/UMG/ArchiveSystem/ArchiveSlotUI.h"

#include "Common/QTBlueprintFunctionLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Core/SubSystems/ArchiveSystem.h"
#include "Kismet/GameplayStatics.h"

void UArchiveSlotUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ArchiveSlotImage)
	{
		ArchiveSlotImage->OnMouseButtonDownEvent.BindUFunction(this, "OnArchiveSlotImageClickEvent");		
	}
}

void UArchiveSlotUI::OnArchiveSlotImageClickEvent()
{
	UArchiveSystem* ArchiveSystem = GetGameInstance()->GetSubsystem<UArchiveSystem>();
	if (ArchiveSystem)
	{
		ArchiveSystem->LoadArchive(SaveSlotIndex);
		UQTBlueprintFunctionLibrary::PlayLoadingScreen(GetWorld(), nullptr, false, 3.0f);
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Desert"));
	}
}

void UArchiveSlotUI::SetArchiveSlotImage(float Opacity)
{
	Opacity = FMath::Clamp(Opacity, 0.0f, 1.0f);
	ArchiveSlotImage->SetOpacity(Opacity);
}

void UArchiveSlotUI::SetSaveDate()
{
	UArchiveSystem* ArchiveSystem = GetGameInstance()->GetSubsystem<UArchiveSystem>();
	if (ArchiveSystem)
	{
		if (ArchiveSystem->IsArchiveExisted(SaveSlotIndex))
		{
			SaveDate->SetText(FText::FromString(ArchiveSystem->SaveDate[SaveSlotIndex]));
		}
	}
}
