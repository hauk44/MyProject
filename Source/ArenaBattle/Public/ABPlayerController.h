// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AABPlayerController();
	
	virtual void PostInitializeComponents() override;

	class UABHUDWidget* GetHUDWidget() const;
	void NPCKill(class AABCharacter* KilledNPC) const;
	void AddGameScore() const;

	void ChangeInputMode(bool bGameMode = true);

	void ShowResultUI();
protected:
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<UABHUDWidget> HUDWidgetClass{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UABGameplayWidget> MenuWidgetClass{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UABGameplayResultWidget> ResultWidgetClass{};
private:
	void OnGamePause();
	
	UPROPERTY()
	UABHUDWidget* HUDWidget{};

	UPROPERTY()
	class AABPlayerState* ABPlayerState{};

	UPROPERTY()
	UABGameplayWidget* MenuWidget{};

	UPROPERTY()
	UABGameplayResultWidget* ResultWidget{};

	FInputModeGameOnly GameInputMode{};
	FInputModeUIOnly UIInputMode{};
};
