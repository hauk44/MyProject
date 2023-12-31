// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ABGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UABGameInstance();

	virtual void Init() override;
	struct FABCharacterData* GetABCharacterData(int32 Level) const;

private:
	UPROPERTY()
	UDataTable* ABCharacterTable{};
};
