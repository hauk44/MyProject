// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"

#include "ABCharacter.h"
#include "ABGameplayResultWidget.h"
#include "ABGameplayWidget.h"
#include "ABGameState.h"
#include "ABHUDWidget.h"
#include "ABPlayerState.h"
#include "ArenaLog.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AABPlayerController::AABPlayerController()
{
	static ConstructorHelpers::FClassFinder<UABHUDWidget>
	UI_HUD_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Book/UI/UI_HUD.UI_HUD_C'"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UABGameplayWidget>
	UI_MENU_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Book/UI/UI_Menu.UI_Menu_C'"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UABGameplayResultWidget>
	UI_RESULT_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Book/UI/UI_Result.UI_Result_C'"));
	if (UI_RESULT_C.Succeeded())
	{
		ResultWidgetClass = UI_RESULT_C.Class;
	}
}

void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

UABHUDWidget* AABPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void AABPlayerController::NPCKill(AABCharacter* KilledNPC) const
{
	ABPlayerState->AddExp(KilledNPC->GetExp());
}

void AABPlayerController::AddGameScore() const
{
	ABPlayerState->AddGameScore();
}

void AABPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void AABPlayerController::ShowResultUI()
{
	auto ABGameState = Cast<AABGameState>(UGameplayStatics::GetGameState(this));
	ABCHECK(ABGameState);
	ResultWidget->BindGameState(ABGameState);
	
	ResultWidget->AddToViewport();
	ChangeInputMode(false);
}

void AABPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), IE_Pressed, this, &AABPlayerController::OnGamePause);
}

void AABPlayerController::OnPossess(APawn* aPawn)
{
	ABLOG_S(Warning);
	Super::OnPossess(aPawn);
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ChangeInputMode(true);

	HUDWidget = CreateWidget<UABHUDWidget>(this, HUDWidgetClass);
	ABCHECK(HUDWidget);
	HUDWidget->AddToViewport(1);

	ABPlayerState = Cast<AABPlayerState>(PlayerState);
	ABCHECK(ABPlayerState);
	HUDWidget->BindPlayerState(ABPlayerState);
	ABPlayerState->OnPlayerStateChanged.Broadcast();

	ResultWidget = CreateWidget<UABGameplayResultWidget>(this, ResultWidgetClass);
	ABCHECK(ResultWidget);
}

void AABPlayerController::OnGamePause()
{
	MenuWidget = CreateWidget<UABGameplayWidget>(this, MenuWidgetClass);
	ABCHECK(MenuWidget);
	MenuWidget->AddToViewport(3);

	SetPause(true);
	ChangeInputMode(false);
}
