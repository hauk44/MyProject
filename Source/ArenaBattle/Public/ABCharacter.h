// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PERINIT,	
	LOADING,	
	READY,	
	DEAD,	
};

UCLASS()
class ARENABATTLE_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();
	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;

	int32 GetExp() const;
	float GetFinalAttackRange() const;
	float GetFinalAttackDamage() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlMode
	{
		GTA,
		DIABLO,
		NPC,
	};
	void SetControlMode(EControlMode NewControlMode);
	EControlMode CurrentControlMode{EControlMode::GTA};
	FVector DirectionToMove{FVector::ZeroVector};
	
	float ArmLengthTo{};
	FRotator ArmRotationTo{FRotator::ZeroRotator};
	float ArmLengthSpeed{};
	float ArmRotationSpeed{};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
		AActor* DamageCauser) override;
	virtual void PossessedBy(AController* NewController) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool CanSetWeapon();
	void SetWeapon(class AABWeapon* NewWeapon);
	
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AABWeapon* CurrentWeapon{};

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UABCharacterStatComponent* CharacterStat{};

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USkeletalMeshComponent* Weapon{};

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* SpringArm{};
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera{};

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget{};

	void Attack();
	FOnAttackEndDelegate OnAttackEnd{};
private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	void ViewChange();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();

	void OnAssetLoadCompleted();
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking{};

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo{};

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn{};

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo{};

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo{};

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange{};

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius{};

	UPROPERTY()
	class UABAnimInstance* ABAnim{};

	FSoftObjectPath CharacterAssetToLoad{nullptr};
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle{};

	int32 AssetIndex{0};

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	ECharacterState CurrentState{};

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool bIsPlayer{};

	UPROPERTY()
	class AABAIController* ABAIController{};

	UPROPERTY()
	class AABPlayerController* ABPlayerController{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
	float DeadTimer{};

	FTimerHandle DeadTimerHandle{};
};
