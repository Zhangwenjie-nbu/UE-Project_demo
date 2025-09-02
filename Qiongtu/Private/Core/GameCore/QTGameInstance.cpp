// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/QTGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Common/QTBlueprintFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Core/GameCore/QTAssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void UQTGameInstance::Init()
{
	Super::Init();
//	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UQTGameInstance::BeginLoadMap);
//	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UQTGameInstance::EndLoadMap);
	LoadingAssets();
}

void UQTGameInstance::BeginLoadMap(const FString& MapName)
{
	UQTBlueprintFunctionLibrary::PlayLoadingScreen(GetWorld(), LoadingWidgetClass, false);
}

void UQTGameInstance::EndLoadMap(UWorld* LoadedWorld)
{
	UQTBlueprintFunctionLibrary::StopLoadingScreen();
}

void UQTGameInstance::ChangeBGM(USoundCue* NewBGM)
{
	if (CurrentBGM)
	{
		CurrentBGM = nullptr;
		if (AudioComponent)
		{
			AudioComponent->SetVolumeMultiplier(0.0f);
			AudioComponent->Stop();
			AudioComponent = nullptr;
		}
	}
	if (NewBGM)
	{
		AudioComponent = UGameplayStatics::SpawnSound2D(this, NewBGM);
		if (AudioComponent == nullptr)
		{
			return;
		}
		AudioComponent->Play();
		CurrentBGM = NewBGM;
		if (BGMSoundClass && BGMSoundMix)
		{
			UQTBlueprintFunctionLibrary::SetVolume(this, BGMSoundMix, BGMSoundClass, UserSettings.GlobalVolume,
				UserSettings.BGMVolume);	
		}
	}
}

void UQTGameInstance::SetAllVolume()
{
	if (UISoundMix && UISoundClass)
	{
		UQTBlueprintFunctionLibrary::SetVolume(this, UISoundMix, UISoundClass, UserSettings.GlobalVolume,
							UserSettings.UIVolume);
	}
}

void UQTGameInstance::LoadingAssets()
{
	if (AssetsToLoad.IsEmpty())
	{
		return;
	}
	UQTAssetManager::Get().LoadPrimaryAssets(AssetsToLoad);
}
