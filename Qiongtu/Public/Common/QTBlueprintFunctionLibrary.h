// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/StreamableManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QTBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API UQTBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UQTBlueprintFunctionLibrary();
	
	/** 显示本机加载画面，例如在地图传输中。如果bPlayUntilStopped为false，它将在播放时间内显示，并自动停止 */
	UFUNCTION(BlueprintCallable, Category = Loading)
	static void PlayLoadingScreen(UWorld* InWorld,TSubclassOf<class UUserWidget> LoadingWidgetClass,
									bool bPlayUntilStopped = true, float PlayTime = 3.0f);

	/** 如果本机加载屏幕是可见的，则关闭它。如果bPlayUntilStopped为真，则必须调用此功能。*/
	UFUNCTION(BlueprintCallable, Category = Loading)
	static void StopLoadingScreen();

	UFUNCTION(BlueprintCallable, Category = UserSettings)
	static void SetVolume(UObject* WorldContext, USoundMix* SoundMix, USoundClass* SoundClass, float GlobalVolume, float CurrentTypeVolume);

	//软引用，同步、异步加载方式
	//同步加载
	template<typename T>
	static T* QTSyncLoadObject(FSoftObjectPath& _ObjectPath);

	//异步加载,加载完成后调用_Handle绑定的回调函数
	static void QTAsyncLoadObject(TArray<FSoftObjectPath> _ObjectPaths, FStreamableDelegate _Handle = FStreamableDelegate());
	
	//拿到异步加载的资源
	template<typename T>
	static T* QTGetAsyncLoadObject(FSoftObjectPath& __ObjectPath);
};

