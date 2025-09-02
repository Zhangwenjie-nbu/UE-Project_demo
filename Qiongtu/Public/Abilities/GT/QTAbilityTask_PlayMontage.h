// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayMontageCallbackProxy.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "QTAbilityTask_PlayMontage.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQTMontageWaitSimpleDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQTOnMontagePlayDelegate, FName, NotifyName);

UCLASS()
class QIONGTU_API UQTAbilityTask_PlayMontage : public UAbilityTask
{
	GENERATED_UCLASS_BODY()
	UPROPERTY(BlueprintAssignable)
	FQTMontageWaitSimpleDelegate	OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FQTMontageWaitSimpleDelegate	OnBlendOut;

	UPROPERTY(BlueprintAssignable)
	FQTMontageWaitSimpleDelegate	OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FQTMontageWaitSimpleDelegate	OnCancelled;

	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	/** 
	 * Start playing an animation montage on the avatar actor and wait for it to finish
	 * If StopWhenAbilityEnds is true, this montage will be aborted if the ability ends normally. It is always stopped when the ability is explicitly cancelled.
	 * On normal execution, OnBlendOut is called when the montage is blending out, and OnCompleted when it is completely done playing
	 * OnInterrupted is called if another montage overwrites this, and OnCancelled is called if the ability or task is cancelled
	 *
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param MontageToPlay The montage to play on the character
	 * @param Rate Change to play the montage faster or slower
	 * @param StartSection If not empty, named montage section to start from
	 * @param bStopWhenAbilityEnds If true, this montage will be aborted if the ability ends normally. It is always stopped when the ability is explicitly cancelled
	 * @param AnimRootMotionTranslationScale Change to modify size of root motion or set to 0 to block it entirely
	 * @param StartTimeSeconds Starting time offset in montage, this will be overridden by StartSection if that is also set
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="QTPlayMontage",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UQTAbilityTask_PlayMontage* CreatePlayMontageAndWaitProxy(UGameplayAbility* OwningAbility,
		FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate = 1.f, FName StartSection = NAME_None, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f);

	virtual void Activate() override;

	/** Called when the ability is asked to cancel from an outside node. What this means depends on the individual task. By default, this does nothing other than ending the task. */
	virtual void ExternalCancel() override;

	virtual FString GetDebugString() const override;

protected:

	virtual void OnDestroy(bool AbilityEnded) override;

	/** Checks if the ability is playing a montage and stops that montage, returns true if a montage was stopped, false if not. */
	bool StopPlayingMontage();

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle InterruptedHandle;

	UPROPERTY()
	UAnimMontage* MontageToPlay;

	UPROPERTY()
	float Rate;

	UPROPERTY()
	FName StartSection;

	UPROPERTY()
	float AnimRootMotionTranslationScale;

	UPROPERTY()
	float StartTimeSeconds;

	UPROPERTY()
	bool bStopWhenAbilityEnds;

	UPROPERTY()
	class UQTGameplayAbility* InAbility;

	TWeakObjectPtr<UAnimInstance> AnimInstancePtr;

//动画通知
	int32 MontageInstanceID;
	
	
	UPROPERTY(BlueprintAssignable)
	FQTOnMontagePlayDelegate OnNotifyBegin;

	UPROPERTY(BlueprintAssignable)
	FQTOnMontagePlayDelegate OnNotifyEnd;
	
	bool IsNotifyValid(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload) const;

	UFUNCTION()
	void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	void OnNotifyEndReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
};
