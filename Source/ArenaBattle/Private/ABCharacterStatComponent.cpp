// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterStatComponent.h"

#include "ABCharacterData.h"
#include "ABGameInstance.h"
#include "ArenaLog.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	Level = 1;
}


// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UABCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void UABCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ABCHECK(ABGameInstance);
	CurrentStatData = ABGameInstance->GetABCharacterData(NewLevel);
	if (CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
	}
	else
	{
		ABLOG(Error, TEXT("Level (%d) data doesn't exist"), NewLevel);
	}
}

void UABCharacterStatComponent::SetDamage(float NewDamage)
{
	ABCHECK(CurrentStatData);
	SetHP(FMath::Clamp<float>(CurrentHP-NewDamage, 0.f, CurrentStatData->MaxHP));
}

void UABCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.f;
		OnHPIsZero.Broadcast();
	}
}

float UABCharacterStatComponent::GetAttack() const
{
	ABCHECK(nullptr != CurrentStatData, 0.f);
	return CurrentStatData->Attack;
}

float UABCharacterStatComponent::GetDropExp() const
{
	return CurrentStatData->DropExp;
}

float UABCharacterStatComponent::GetHPRatio() const
{
	ABCHECK(nullptr != CurrentStatData, 0.f);

	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.f : (CurrentHP / CurrentStatData->MaxHP);
}

