// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/UMG/GameSettings/MainGameSettingsUI.h"

#include "Common/QTBlueprintFunctionLibrary.h"
#include "Components/Button.h"
#include "Core/GameCore/QTPlayerController.h"
#include "Core/SubSystems/ArchiveSystem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Common/UMG/ArchiveSystem/ArchiveInterfaceUI.h"
#include "UI/GameUI/UMG/InventoryUI.h"
#include "UI/GameUI/UMG/EquipmentUI.h"
#include "UI/Common/UMG/GameSettings/GameSettingsSystemUI.h"

void UMainGameSettingsUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UGameplayStatics::SetGamePaused(this, true);
	OptionBtn->OnClicked.AddDynamic(this, &UMainGameSettingsUI::OnOptionBtnClick);
	InventoryBtn->OnClicked.AddDynamic(this, &UMainGameSettingsUI::OnInventoryBtnClick);
	SaveGameBtn->OnClicked.AddDynamic(this, &UMainGameSettingsUI::OnSaveGameBtnClick);
	LoadGameBtn->OnClicked.AddDynamic(this, &UMainGameSettingsUI::OnLoadGameBtnClick);
	ZhujiemianBtn->OnClicked.AddDynamic(this, &UMainGameSettingsUI::OnZhujiemianBtnClick);
	QuitGameBtn->OnClicked.AddDynamic(this, &UMainGameSettingsUI::OnQuitGameBtnClick);
	EquipmentBtn->OnClicked.AddDynamic(this, &UMainGameSettingsUI::OnEquipmentBtnClick);
}

void UMainGameSettingsUI::OnOptionBtnClick()
{
	if (OptionUIClass)
	{
		ChangeWidget(OptionUIClass);
	}
}

void UMainGameSettingsUI::OnEquipmentBtnClick()
{
	if (EquipmentUIClass)
	{
		ChangeWidget(EquipmentUIClass);
	}
}

void UMainGameSettingsUI::OnInventoryBtnClick()
{
	if (InventoryUIClass)
	{
		ChangeWidget(InventoryUIClass);
	}

}

void UMainGameSettingsUI::OnSaveGameBtnClick()
{
	UArchiveSystem* ArchiveSystem = GetGameInstance()->GetSubsystem<UArchiveSystem>();
	if (ArchiveSystem)
	{
		ArchiveSystem->SaveArchive();
	}
}

void UMainGameSettingsUI::OnLoadGameBtnClick()
{
	if (ArchiveInterfaceUIClass)
	{
		ChangeWidget(ArchiveInterfaceUIClass);
	}
}

void UMainGameSettingsUI::OnZhujiemianBtnClick()
{
	UQTBlueprintFunctionLibrary::PlayLoadingScreen(GetWorld(), nullptr, false, 3.0f);
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("HallMap"));
}

void UMainGameSettingsUI::OnQuitGameBtnClick()
{
	AQTPlayerController* PC = GetGamePlayerController();
	if (PC)
	{
		PC->ConsoleCommand("quit");
	}
}

void UMainGameSettingsUI::RemoveWidget()
{
	UGameplayStatics::SetGamePaused(this, false);
	Super::RemoveWidget();
}
