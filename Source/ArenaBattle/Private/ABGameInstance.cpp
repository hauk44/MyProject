// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameInstance.h"
#include "ABCharacterData.h"
#include "ArenaLog.h"

UABGameInstance::UABGameInstance()
{
	const FString CharaterDataPath = TEXT("/Script/Engine.DataTable'/Game/Book/GameData/ABCharacterData.ABCharacterData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ABCHARACTER(*CharaterDataPath);
	ABCHECK(DT_ABCHARACTER.Succeeded());
	ABCharacterTable = DT_ABCHARACTER.Object;
	ABCHECK(ABCharacterTable->GetRowMap().Num() > 0);
}

void UABGameInstance::Init()
{
	Super::Init();
}

FABCharacterData* UABGameInstance::GetABCharacterData(int32 Level) const
{
	return ABCharacterTable->FindRow<FABCharacterData>(*FString::FromInt(Level), TEXT(""));
}
