// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangeDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void InitializeComponent() override;

	void SetNewLevel(int32 NewLevel);
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);
	float GetAttack() const;
	float GetHPRatio() const;
	float GetDropExp() const;

	FOnHPIsZeroDelegate OnHPIsZero{};
	FOnHPChangeDelegate OnHPChanged{};

private:
	struct FABCharacterData* CurrentStatData{nullptr};

	UPROPERTY(EditInstanceOnly, Category = STAT, Meta = (AllowPrivateAccess = true))
	int32 Level{};

	UPROPERTY(Transient, VisibleInstanceOnly, Category = STAT, Meta = (AllowPrivateAccess = true))
	float CurrentHP{};
};
