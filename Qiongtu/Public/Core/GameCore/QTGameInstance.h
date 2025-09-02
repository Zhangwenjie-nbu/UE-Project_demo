// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Items/Item/QTTypes.h"
#include "QTGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API UQTGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	
public:
	virtual void Init() override;

	void BeginLoadMap(const FString& MapName);
	void EndLoadMap(UWorld* LoadedWorld);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> LoadingWidgetClass;

//声音设置
protected:
	UPROPERTY()
	class USoundCue* CurrentBGM;
	UPROPERTY()
	UAudioComponent* AudioComponent;	
	
public:
	UFUNCTION(BlueprintCallable, Category = "UserSettings")
	void ChangeBGM(class USoundCue* NewBGM);
	UFUNCTION(BlueprintCallable, Category = "UserSettings")
	void SetAllVolume();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UserSettings")
	USoundMix* BGMSoundMix;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UserSettings")
	USoundClass* BGMSoundClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UserSettings")
	USoundMix* UISoundMix;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UserSettings")
	USoundClass* UISoundClass;

	UPROPERTY(BlueprintReadWrite)
	FQTUserSettings UserSettings;

	UPROPERTY(EditDefaultsOnly)
	TArray<FPrimaryAssetId> AssetsToLoad;
	void LoadingAssets();

};
