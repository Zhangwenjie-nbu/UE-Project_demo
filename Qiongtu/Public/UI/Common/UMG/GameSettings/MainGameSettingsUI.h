// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UserWidgetBase.h"
#include "MainGameSettingsUI.generated.h"

/**
 * 
 */
UCLASS()
class QIONGTU_API UMainGameSettingsUI : public UUserWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	UPROPERTY(EditDefaultsOnly, Category = "ChildWidgetClass")
	TSubclassOf<class UArchiveInterfaceUI> ArchiveInterfaceUIClass;
	UPROPERTY(EditDefaultsOnly, Category = "ChildWidgetClass")
	TSubclassOf<class UInventoryUI> InventoryUIClass;
	UPROPERTY(EditDefaultsOnly, Category = "ChildWidgetClass")
	TSubclassOf<class UGameSettingsSystemUI> OptionUIClass;
	UPROPERTY(EditDefaultsOnly, Category = "ChildWidgetClass")
	TSubclassOf<class UEquipmentUI> EquipmentUIClass;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* OptionBtn;
	UFUNCTION()
	void OnOptionBtnClick();

	UPROPERTY(meta = (BindWidget))
	class UButton* EquipmentBtn;
	UFUNCTION()
	void OnEquipmentBtnClick();

	UPROPERTY(meta = (BindWidget))
	class UButton* InventoryBtn;
	UFUNCTION()
	void OnInventoryBtnClick();

	UPROPERTY(meta = (BindWidget))
	class UButton* SaveGameBtn;
	UFUNCTION()
	void OnSaveGameBtnClick();

	UPROPERTY(meta = (BindWidget))
	class UButton* LoadGameBtn;
	UFUNCTION()
	void OnLoadGameBtnClick();

	UPROPERTY(meta = (BindWidget))
	class UButton* ZhujiemianBtn;
	UFUNCTION()
	void OnZhujiemianBtnClick();

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitGameBtn;
	UFUNCTION()
	void OnQuitGameBtnClick();
public:
	virtual void RemoveWidget() override;
};
