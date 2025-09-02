// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameCore/QTPlayerController.h"

#include "KismetAnimationLibrary.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/QTPlayerBase.h"
#include "Components/EquipmentComponent.h"
#include "UI/GameUI/UMG/InventoryUI.h"
#include "UI/Common/UMG/GameSettings/MainGameSettingsUI.h"
#include "Components/InventoryComp.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item/ItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Core/UserWidgetBase.h"

AQTPlayerController::AQTPlayerController()
	: TurnRate(35.0f)
	, LookUpRate(35.0f)
	, bInventoryOpened(false)
{
	CurrentWidget = nullptr;
	InventoryComp = CreateDefaultSubobject<UInventoryComp>(TEXT("InventoryComp"));
	EquipmentComp = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComp"));
}

void AQTPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
	SetPlayerHUD();
}

void AQTPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SetLockSystem();
}

void AQTPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (Cast<AQTCharacterBase>(InPawn))
	{
		MyPawn = Cast<AQTCharacterBase>(InPawn);
		SetTargetLockUIVisible(MyPawn, false);
	}
}


void AQTPlayerController::SetLockSystem()
{
	AQTPlayerBase* MyCharacter = Cast<AQTPlayerBase>(GetPawn());
	if (MyCharacter == nullptr)
	{
		return;
	}
	if (MyPawn->LockedTargetEnemy)
	{
		if (MyCharacter->MovementStates.RotationMode == EQTRotationMode::ELookingDirection)
		{
			LockEnemy(MyPawn->LockedTargetEnemy);
		}
	}
	else
	{
		if (MyCharacter->MovementStates.RotationMode != EQTRotationMode::EVelocityDirection)
		{
			MyCharacter->MovementStates.RotationMode = EQTRotationMode::EVelocityDirection;
		}
	}
}


void AQTPlayerController::ChangeWidget(const TSubclassOf<UUserWidgetBase> NewWidgetClass)
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveWidget();
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidgetBase>(GetWorld(), NewWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

UInventoryComp* AQTPlayerController::GetInventoryComp()
{
	if (InventoryComp)
	{
		return InventoryComp;
	}
	return nullptr;
}

UEquipmentComponent* AQTPlayerController::GetEquipmentComp()
{
	if (EquipmentComp)
	{
		return EquipmentComp;
	}
	return nullptr;
}

void AQTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("Turn", this, &AQTPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &AQTPlayerController::LookUp);

	InputComponent->BindAction("ESC", EInputEvent::IE_Pressed, this, &AQTPlayerController::ESCPressed).bExecuteWhenPaused = true;
	InputComponent->BindAction("Pickup", IE_Pressed, this, &AQTPlayerController::PickUpItem);
	InputComponent->BindAction("LockedEnemy", IE_Pressed, this, &AQTPlayerController::LockedEnemy);
	InputComponent->BindAction("LeftLocked", IE_Pressed, this, &AQTPlayerController::LeftLocked);
	InputComponent->BindAction("RightLocked", IE_Pressed, this, &AQTPlayerController::RightLocked);
	InputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &AQTPlayerController::SwitchCurrentWeapon);
	InputComponent->BindAction("SwitchConsumable", IE_Pressed, this, &AQTPlayerController::SwitchCurrentConsumable);
}


void AQTPlayerController::ESCPressed()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveChildWidget();
		if (CurrentWidget == nullptr)
		{
			SetInputMode(FInputModeGameOnly());
			bShowMouseCursor = false;
		}
	}
	else
	{
		ChangeWidget(MainGameSettingsWidgetClass);
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
	}
}

void AQTPlayerController::LockedEnemy()
{
	AQTPlayerBase* MyCharacter = Cast<AQTPlayerBase>(GetPawn());
	if (MyCharacter)
	{
		if (MyCharacter->MovementStates.RotationMode != EQTRotationMode::ELookingDirection)
		{
			MyCharacter->SetRotationMode(EQTRotationMode::ELookingDirection);
			GetRecentlyRenderedEnemy();
			MyPawn->LockedTargetEnemy = FindForwardEnemy();
		}
		else
		{
			MyCharacter->SetRotationMode(EQTRotationMode::EVelocityDirection);
			if (AQTCharacterBase* TargetCharacter = Cast<AQTCharacterBase>(MyPawn->LockedTargetEnemy))
			{
				SetTargetLockUIVisible(TargetCharacter, false);
			}
			MyPawn->LockedTargetEnemy = nullptr;
			LockedEnemies.Empty();
		}
	}
}

void AQTPlayerController::LeftLocked()
{
	AQTPlayerBase* MyCharacter = Cast<AQTPlayerBase>(GetPawn());
	if (MyCharacter)
	{
		if (MyCharacter->MovementStates.RotationMode == EQTRotationMode::ELookingDirection)
		{
			LockedEnemies.Empty();
			GetRecentlyRenderedEnemy();
			MyPawn->LockedTargetEnemy = FindLeftEnemy();
		}
	}
}

void AQTPlayerController::RightLocked()
{
	AQTPlayerBase* MyCharacter = Cast<AQTPlayerBase>(GetPawn());
	if (MyCharacter)
	{
		if (MyCharacter->MovementStates.RotationMode == EQTRotationMode::ELookingDirection)
		{
			LockedEnemies.Empty();
			GetRecentlyRenderedEnemy();
			MyPawn->LockedTargetEnemy = FindRightEnemy();
		}
	}
}

void AQTPlayerController::GetRecentlyRenderedEnemy()
{
	const FVector PlayerLocation = GetPawn()->GetActorLocation();
	UGameplayStatics::GetAllActorsWithTag(this, FName("Enemy"), LockedEnemies);
	for (AActor* Tmp : LockedEnemies)
	{
		FVector Director = Tmp->GetActorLocation() - PlayerLocation;
		const float TmpDegree = UKismetAnimationLibrary::CalculateDirection(Director, GetControlRotation());
		const float Distance = (PlayerLocation - Tmp->GetActorLocation()).Length();
		if (FMath::Abs(TmpDegree) > 75.f || Distance > LockedDistance)
		{
			LockedEnemies.Remove(Tmp);
		}
	}
}

AQTCharacterBase* AQTPlayerController::FindForwardEnemy()
{
	if (GetPawn() == nullptr)
	{
		return nullptr;
	}
	AQTCharacterBase* ForwardEnemy = nullptr;
	float MinimumDegree = 180.f;
	for (AActor* Tmp : LockedEnemies)
	{
		const FVector PlayerLocation = GetPawn()->GetActorLocation();
		FVector Director = Tmp->GetActorLocation() - PlayerLocation;
		const float TmpDegree = UKismetAnimationLibrary::CalculateDirection(Director, GetControlRotation());
		if (FMath::Abs(TmpDegree) < MinimumDegree)
		{
			MinimumDegree = FMath::Abs(TmpDegree);
			ForwardEnemy = Cast<AQTCharacterBase>(Tmp);
		}
	}
	if (AQTCharacterBase* TargetCharacter = Cast<AQTCharacterBase>(MyPawn->LockedTargetEnemy))
	{
		if (ForwardEnemy != nullptr)
		{
			SetTargetLockUIVisible(TargetCharacter, false);
		}
	}
	if (AQTCharacterBase* TargetCharacter = Cast<AQTCharacterBase>(ForwardEnemy))
	{
		SetTargetLockUIVisible(TargetCharacter, true);
	}
	if (ForwardEnemy == nullptr)
	{
		return MyPawn->LockedTargetEnemy;
	}
	return ForwardEnemy;
}

AQTCharacterBase* AQTPlayerController::FindLeftEnemy()
{
	if (GetPawn() == nullptr)
	{
		return nullptr;
	}
	AQTCharacterBase* LeftEnemy = nullptr;
	float MinimumDegree = 180.f;
	for (AActor* Tmp : LockedEnemies)
	{
		const FVector PlayerLocation = GetPawn()->GetActorLocation();
		FVector Director = Tmp->GetActorLocation() - PlayerLocation;
		const float TmpDegree = UKismetAnimationLibrary::CalculateDirection(Director, GetControlRotation());
		if (TmpDegree < 0.f && FMath::Abs(TmpDegree) < MinimumDegree)
		{
			MinimumDegree = FMath::Abs(TmpDegree);
			LeftEnemy = Cast<AQTCharacterBase>(Tmp);
		}
	}
	if (AQTCharacterBase* TargetCharacter = Cast<AQTCharacterBase>(MyPawn->LockedTargetEnemy))
	{
		if (LeftEnemy != nullptr)
		{
			SetTargetLockUIVisible(TargetCharacter, false);
		}
	}
	if (AQTCharacterBase* TargetCharacter = Cast<AQTCharacterBase>(LeftEnemy))
	{
		SetTargetLockUIVisible(TargetCharacter, true);
	}
	if (LeftEnemy == nullptr)
	{
		return MyPawn->LockedTargetEnemy;
	}
	return LeftEnemy;
}

AQTCharacterBase* AQTPlayerController::FindRightEnemy()
{
	if (GetPawn() == nullptr)
	{
		return nullptr;
	}
	AQTCharacterBase* RightEnemy = nullptr;
	float MinimumDegree = 180.f;
	for (AActor* Tmp : LockedEnemies)
	{
		const FVector PlayerLocation = GetPawn()->GetActorLocation();
		FVector Director = Tmp->GetActorLocation() - PlayerLocation;
		const float TmpDegree = UKismetAnimationLibrary::CalculateDirection(Director, GetControlRotation());
		if (TmpDegree > 0.f && TmpDegree < MinimumDegree)
		{
			MinimumDegree = FMath::Abs(TmpDegree);
			RightEnemy = Cast<AQTCharacterBase>(Tmp);
		}
	}
	if (AQTCharacterBase* TargetCharacter = Cast<AQTCharacterBase>(MyPawn->LockedTargetEnemy))
	{
		if (RightEnemy != nullptr)
		{
			SetTargetLockUIVisible(TargetCharacter, false);
		}
	}
	if (AQTCharacterBase* TargetCharacter = Cast<AQTCharacterBase>(RightEnemy))
	{
		SetTargetLockUIVisible(TargetCharacter, true);
	}
	if (RightEnemy == nullptr)
	{
		return MyPawn->LockedTargetEnemy;
	}
	return RightEnemy;
}

void AQTPlayerController::LockEnemy(AQTCharacterBase* Enemy)
{
	if (Enemy == nullptr || GetPawn() == nullptr)
	{
		return;
	}
	FRotator _TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), Enemy->GetActorLocation());
	_TargetRotation.Roll = 0.f;
	FRotator TargetRotation = GetControlRotation();
	TargetRotation = FMath::RInterpConstantTo(TargetRotation, _TargetRotation,
					UGameplayStatics::GetWorldDeltaSeconds(this), 200.f);
	SetControlRotation(TargetRotation);
}


void AQTPlayerController::SetPlayerHUD()
{
	if (!IsLocalController())
	{
		return;
	}
	if (PlayerHUDClass)
	{
		if (UUserWidgetBase* HUD = CreateWidget<UUserWidgetBase>(GetWorld(), PlayerHUDClass))
		{
			HUD->AddToViewport();
		}
	}
}

void AQTPlayerController::Turn(float Value)
{
	const float LocalRate = FMath::Clamp(TurnRate / BaseRate, 0.0f, 2.0f);
	const float Rate = Value * LocalRate;
	AddYawInput(Rate);
}

void AQTPlayerController::LookUp(float Value)
{
	const float LocalRate = FMath::Clamp(LookUpRate / BaseRate, 0.0f, 2.0f);
	const float Rate = Value * LocalRate;
	AddPitchInput(Rate);
}

void AQTPlayerController::PickUpItem()
{
	AQTPlayerBase* MyCharacter = Cast<AQTPlayerBase>(GetPawn());
	if (MyCharacter == nullptr)
	{
		return;
	}
	const FVector Start = MyCharacter->GetCameraComp()->GetComponentLocation();
	const FVector End = MyCharacter->GetCameraComp()->GetForwardVector() * 500.0f + Start;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	ObjectType.Emplace(PickUpObjectTypeQuery);
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(MyCharacter);

	FHitResult HitResult;
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Start, End, 50.0f, ObjectType,
		false, ActorsToIgnore,	EDrawDebugTrace::Type::None, HitResult, true))
	{
		AItemBase* PickUpItem = Cast<AItemBase>(HitResult.GetActor());
		if (PickUpItem == nullptr)
		{
			return;
		}
		if (PickUpItem->bIsCloseToPlayer)
		{
			GetInventoryComp()->AddItemToInventory(PickUpItem);
		}
	}
}

void AQTPlayerController::SwitchCurrentWeapon()
{
	if (GetEquipmentComp())
	{
		GetEquipmentComp()->SetCurrentWeapon();
	}
}

void AQTPlayerController::SwitchCurrentConsumable()
{
	if (GetEquipmentComp())
	{
		GetEquipmentComp()->SetCurrentConsumable();
	}
}
