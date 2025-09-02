// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/QTBlueprintFunctionLibrary.h"
#include "QTLoadingScreen.h"
#include "Blueprint/UserWidget.h"
#include "Core/GameCore/QTAssetManager.h"
#include "Kismet/GameplayStatics.h"

UQTBlueprintFunctionLibrary::UQTBlueprintFunctionLibrary()
{
}

void UQTBlueprintFunctionLibrary::PlayLoadingScreen(UWorld* InWorld, TSubclassOf<UUserWidget> LoadingWidgetClass,
	bool bPlayUntilStopped, float PlayTime)
{
	UUserWidget* LoadingWidget = nullptr;
	LoadingWidgetClass ? LoadingWidget = CreateWidget<UUserWidget>(InWorld, LoadingWidgetClass) :
							LoadingWidget = CreateWidget<UUserWidget>(InWorld, LoadClass<UUserWidget>(InWorld,
							TEXT("WidgetBlueprint'/Game/Blueprint/UI/LoadingScreen/WBP_LoadingScreen.WBP_LoadingScreen_C'")));
	if (!LoadingWidget)
	{
		return;
	}
	IQTLoadingScreenModule& LoadingScreenModule = IQTLoadingScreenModule::Get();
	LoadingScreenModule.StartInGameLoadingScreen(bPlayUntilStopped, PlayTime, LoadingWidget);
}

void UQTBlueprintFunctionLibrary::StopLoadingScreen()
{
	IQTLoadingScreenModule& LoadingScreenModule = IQTLoadingScreenModule::Get();
	LoadingScreenModule.StopInGameLoadingScreen();
}

void UQTBlueprintFunctionLibrary::SetVolume(UObject* WorldContext, USoundMix* SoundMix, USoundClass* SoundClass, float GlobalVolume,
	float CurrentTypeVolume)
{
	const float FinalVolume = FMath::Clamp(GlobalVolume, 0.01f, 1.0f) * FMath::Clamp(CurrentTypeVolume, 0.01f, 1.0f);
	UGameplayStatics::SetSoundMixClassOverride(WorldContext, SoundMix, SoundClass, FinalVolume, 1, 0.0f);
}

template <typename T>
T* UQTBlueprintFunctionLibrary::QTSyncLoadObject(FSoftObjectPath& _ObjectPath)
{
	if (_ObjectPath.IsValid())
	{
		return Cast<T>(UQTAssetManager::Get().GetStreamableManager().LoadSynchronous(_ObjectPath));
	}
	return nullptr;
}

void UQTBlueprintFunctionLibrary::QTAsyncLoadObject(TArray<FSoftObjectPath> _ObjectPaths,
	FStreamableDelegate _Handle)
{
	UQTAssetManager::Get().GetStreamableManager().RequestAsyncLoad(_ObjectPaths, _Handle);
}

template <typename T>
T* UQTBlueprintFunctionLibrary::QTGetAsyncLoadObject(FSoftObjectPath& _ObjectPath)
{
	if (_ObjectPath.IsValid())
	{
		return Cast<T>(FSoftObjectPtr(_ObjectPath).Get());
	}
	return nullptr;
}
