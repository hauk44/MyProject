// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fountain.generated.h"

UCLASS()
class ARENABATTLE_API AFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void PostInitializeComponents() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Body{};

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Water{};

	UPROPERTY(VisibleAnywhere)
	class UPointLightComponent *Light{};

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent *Splash{};

	UPROPERTY(EditAnywhere, Category=ID)
	int32 ID{};

	UPROPERTY(VisibleAnywhere)
	class URotatingMovementComponent* Movement{};
private:
	UPROPERTY(EditAnywhere, Category=Stat, Meta = (AllowPrivateAccess = true))
	float RotateSpeed{};
};
