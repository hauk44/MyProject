// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABCharacterData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct ARENABATTLE_API FABCharacterData : public FTableRowBase
{
	GENERATED_BODY()
	
	FABCharacterData() : Level(1), MaxHP(100.f), Attack(10.f), DropExp(10), NextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp{};
};
