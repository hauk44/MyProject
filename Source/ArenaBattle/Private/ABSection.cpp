// Fill out your copyright notice in the Description page of Project Settings.


#include "ABSection.h"

#include "ABCharacter.h"
#include "ABGameMode.h"
#include "ABItemBox.h"
#include "ABPlayerController.h"
#include "ArenaLog.h"

// Sets default values
AABSection::AABSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;

	FString AssetPath{TEXT("/Script/Engine.StaticMesh'/Game/Book/StaticMesh/SM_SQUARE.SM_SQUARE'")};
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SQUARE(*AssetPath);
	if (SM_SQUARE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_SQUARE.Object);
	}
	else
	{
		ABLOG(Error, TEXT("Failed to load staticmesh asset. : %s"), *AssetPath);
	}

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetBoxExtent(FVector{775.f, 775.f, 300.f});
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetRelativeLocation(FVector{0.f, 0.f, 250.f});
	Trigger->SetCollisionProfileName(TEXT("ABTrigger"));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABSection::OnTriggerBeginOverlap);
	
	FString GateAssetPath{TEXT("/Script/Engine.StaticMesh'/Game/Book/StaticMesh/SM_GATE.SM_GATE'")};
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE(*GateAssetPath);
	if (!SM_GATE.Succeeded())
	{
		ABLOG(Error, TEXT("Failed to load staticmesh asset. : %s"), *GateAssetPath);
	}

	static FName GateSockets[] = {
		{TEXT("+XGate")},
		{TEXT("-XGate")},
		{TEXT("+YGate")},
		{TEXT("-YGate")},
	};
	for (FName GateSocket : GateSockets)
	{
		ABCHECK(Mesh->DoesSocketExist(GateSocket));
		UStaticMeshComponent* NewGate {CreateDefaultSubobject<UStaticMeshComponent>(*GateSocket.ToString())};
		NewGate->SetStaticMesh(SM_GATE.Object);
		NewGate->SetupAttachment(RootComponent, GateSocket);
		NewGate->SetRelativeLocation(FVector(0.f, -80.5f, 0.f));
		GateMeshes.Add(NewGate);

		UBoxComponent* NewGateTrigger = CreateDefaultSubobject<UBoxComponent>(
			*GateSocket.ToString().Append(TEXT("Trigger")));
        NewGateTrigger->SetBoxExtent(FVector{100.f, 100, 300.f});
        NewGateTrigger->SetupAttachment(RootComponent, GateSocket);
        NewGateTrigger->SetRelativeLocation(FVector{70.f, 0.f, 250.f});
        NewGateTrigger->SetCollisionProfileName(TEXT("ABTrigger"));
		GateTriggers.Add(NewGateTrigger);

		NewGateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABSection::OnGateTriggerBeginOverlap);
		NewGateTrigger->ComponentTags.Add(GateSocket);
	}

	bNoBattle = false;

	EnemySpawnTime = 2.f;
	ItemBoxSpawnTime = 5.f;
}

void AABSection::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

// Called when the game starts or when spawned
void AABSection::BeginPlay()
{
	Super::BeginPlay();

	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

// Called every frame
void AABSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AABSection::SetState(ESectionState NewState)
{
	switch (NewState)
	{
	case ESectionState::READY:
		{
			Trigger->SetCollisionProfileName(TEXT("ABTrigger"));
			for (UBoxComponent* GateTrigger : GateTriggers)
			{
				GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
			}
			OperateGates(true);
			break;
		}
	case ESectionState::BATTLE:
		{
			Trigger->SetCollisionProfileName(TEXT("NoCollision"));
			for (UBoxComponent* GateTrigger : GateTriggers)
			{
				GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
			}
			OperateGates(false);

			GetWorld()->GetTimerManager().SetTimer(SpawnNPCTimerHandle,
				FTimerDelegate::CreateUObject(this, &AABSection::OnNPCSpawn), EnemySpawnTime, false);

			GetWorld()->GetTimerManager().SetTimer(SpawnItemBoxTimerHandle,
				FTimerDelegate::CreateLambda([this]()->void
				{
					FVector2d RandXY = FMath::RandPointInCircle(600.f);
					GetWorld()->SpawnActor<AABItemBox>(GetActorLocation()+
						FVector(RandXY, 30.f), FRotator::ZeroRotator);
				}), ItemBoxSpawnTime, false);
			break;
		}
	case ESectionState::COMPLETE:
		{
			Trigger->SetCollisionProfileName(TEXT("NoCollision"));
			for (UBoxComponent* GateTrigger : GateTriggers)
			{
				GateTrigger->SetCollisionProfileName(TEXT("ABTrigger"));
			}
			OperateGates(true);
			break;
		}
	}

	CurrentState = NewState;
}

void AABSection::OperateGates(bool bOpen)
{
	for (UStaticMeshComponent* Gate : GateMeshes)
	{
		Gate->SetRelativeRotation(bOpen ? FRotator{0.f, -90.f, 0.f} : FRotator::ZeroRotator);
	}
}

void AABSection::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CurrentState == ESectionState::READY)
	{
		SetState(ESectionState::BATTLE);
	}
}

void AABSection::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABCHECK(OverlappedComponent->ComponentTags.Num() == 1);
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	if (!Mesh->DoesSocketExist(SocketName)) return;

	FVector NewLocation = Mesh->GetSocketLocation(SocketName);

	TArray<FOverlapResult> OverlapResults{};
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllObjects);
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		ObjectQueryParam,
		FCollisionShape::MakeSphere(775.f),
		CollisionQueryParam
		);

	if (!bResult)
	{
		auto NewSection = GetWorld()->SpawnActor<AABSection>(NewLocation, FRotator::ZeroRotator);
	}
	else
	{
		ABLOG(Warning, TEXT("New section area is not empty."));
	}
}

void AABSection::OnNPCSpawn()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnNPCTimerHandle);
	auto KeyNPC = GetWorld()->SpawnActor<AABCharacter>(GetActorLocation() +
		FVector::UpVector * 88.f, FRotator::ZeroRotator);
	if (KeyNPC)
	{
		KeyNPC->OnDestroyed.AddDynamic(this, &AABSection::OnKeyNPCDestroyed);
	}
}

void AABSection::OnKeyNPCDestroyed(AActor* DestroyedActor)
{
	const auto ABCharacter = Cast<AABCharacter>(DestroyedActor);
	ABCHECK(ABCharacter);

	const auto ABPlayerController = Cast<AABPlayerController>(ABCharacter->LastHitBy);
	ABCHECK(ABPlayerController);

	const auto ABGameMode = Cast<AABGameMode>(GetWorld()->GetAuthGameMode());
	ABCHECK(ABGameMode);
	ABGameMode->AddScore(ABPlayerController);
	
	SetState(ESectionState::COMPLETE);
}

