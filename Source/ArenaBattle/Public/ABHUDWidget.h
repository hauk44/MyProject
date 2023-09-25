// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ABHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(class UABCharacterStatComponent* CharacterStat);
	void BindPlayerState(class AABPlayerState* PlayerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();

private:
	TWeakObjectPtr<UABCharacterStatComponent> CurrentCharacterStat{};
	TWeakObjectPtr<AABPlayerState> CurrentPlayerState{};

	UPROPERTY()
	class UProgressBar* HPBar{};

	UPROPERTY()
	UProgressBar* ExpBar{};

	UPROPERTY()
	class UTextBlock* PlayerName{};

	UPROPERTY()
	UTextBlock* PlayerLevel{};

	UPROPERTY()
	UTextBlock* CurrentScore{};

	UPROPERTY()
	UTextBlock* HighScore{};
};
