// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"

#include "ABCharacter.h"
#include "ABGameState.h"
#include "ABPlayerController.h"
#include "ABPlayerState.h"
#include "ArenaLog.h"
#include "Engine/PawnIterator.h"

AABGameMode::AABGameMode()
{
	DefaultPawnClass = AABCharacter::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
	PlayerStateClass = AABPlayerState::StaticClass();
	GameStateClass = AABGameState::StaticClass();

	ScoreToClear = 2;
}

void AABGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABGameState = Cast<AABGameState>(GameState);
}

void AABGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto ABPlayerState = Cast<AABPlayerState>(NewPlayer->PlayerState);
	ABCHECK(ABPlayerState);
	ABPlayerState->InitPlayerData();
}

void AABGameMode::AddScore(AABPlayerController* ScoredPlayer)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto ABPlayerController = Cast<AABPlayerController>(It->Get());
		if (ABPlayerController && ScoredPlayer == ABPlayerController)
		{
			ABPlayerController->AddGameScore();
			break;
		}
	}
	ABGameState->AddGameScore();

	if (GetScore() >= ScoreToClear)
	{
		ABGameState->SetGameCleared();

		for (TActorIterator<APawn> It(GetWorld()); It; ++It)
		{
			(*It)->TurnOff();
		}
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			const auto ABPlayerController = Cast<AABPlayerController>(It->Get());
			if (ABPlayerController)
			{
				ABPlayerController->ShowResultUI();
			}
		}
	}
}

int32 AABGameMode::GetScore() const
{
	return ABGameState->GetTotalGameScore();
}
