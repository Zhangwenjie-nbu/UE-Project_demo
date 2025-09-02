// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Sets/QTAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Core/GameCore/QTCharacterBase.h"
#include "GameFramework/Controller.h"

UQTAttributeSet::UQTAttributeSet()
		: Health(100.f)
		, MaxHealth(100.f)
{
	
}

void UQTAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UQTAttributeSet, Health);
	DOREPLIFETIME(UQTAttributeSet, MaxHealth);
}


void UQTAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
}

void UQTAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	// Compute the delta between old and new, if it is available
	float DeltaValue = 0;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		// If this was additive, store the raw delta value to be passed along later
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	AQTCharacterBase* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Cast<AController>(Data.Target.AbilityActorInfo->PlayerController.Get());
		TargetCharacter = Cast<AQTCharacterBase>(TargetActor);
	}

	// if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	// {
	// 	// Get the Source actor
	// 	AActor* SourceActor = nullptr;
	// 	AController* SourceController = nullptr;
	// 	AQTCharacterBase* SourceCharacter = nullptr;
	// 	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	// 	{
	// 		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
	// 		SourceController = Source->AbilityActorInfo->PlayerController.Get();
	// 		if (SourceController == nullptr && SourceActor != nullptr)
	// 		{
	// 			if (APawn* Pawn = Cast<APawn>(SourceActor))
	// 			{
	// 				SourceController = Pawn->GetController();
	// 			}
	// 		}
	//
	// 		// Use the controller to find the source pawn
	// 		if (SourceController)
	// 		{
	// 			SourceCharacter = Cast<AQTCharacterBase>(SourceController->GetPawn());
	// 		}
	// 		else
	// 		{
	// 			SourceCharacter = Cast<AQTCharacterBase>(SourceActor);
	// 		}
	//
	// 		// Set the causer actor based on context if it's set
	// 		if (Context.GetEffectCauser())
	// 		{
	// 			SourceActor = Context.GetEffectCauser();
	// 		}
	// 	}

	// 	// Try to extract a hit result
	// 	FHitResult HitResult;
	// 	if (Context.GetHitResult())
	// 	{
	// 		HitResult = *Context.GetHitResult();
	// 	}
	// }
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Handle other health changes such as from healing or direct modifiers
		// First clamp it
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		if (TargetCharacter)
		{
			// Call for all health changes
			TargetCharacter->HandleHealthChangedOnServer(GetHealth(), GetMaxHealth());
		}
	}

}


void UQTAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void UQTAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UQTAttributeSet, Health, OldValue);
}
	
void UQTAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UQTAttributeSet, Health, OldValue);
}
