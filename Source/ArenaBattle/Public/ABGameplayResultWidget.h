// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABGameplayWidget.h"
#include "ABGameplayResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABGameplayResultWidget : public UABGameplayWidget
{
	GENERATED_BODY()
public:
	void BindGameState(class AABGameState* GameState);
	
protected:
	virtual void NativeConstruct() override;

private:
	TWeakObjectPtr<AABGameState> CurrentGameState{};
};
