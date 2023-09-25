// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABAIController : public AAIController
{
	GENERATED_BODY()
public:
	AABAIController();

	inline static const FName HomePosKey{TEXT("HomePos")};
	inline static const FName PatrolPosKey{TEXT("PatrolPos")};
	inline static const FName TargetKey{TEXT("Target")};

    void RunAI();
    void StopAI();
protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	UBehaviorTree* BTAsset;

	UPROPERTY()
	UBlackboardData* BBAsset;
};
