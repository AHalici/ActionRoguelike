// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	
	// This lets us assign a class
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
    UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_BlackHoleAttack;
	FTimerHandle TimerHandle_Dash;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void Jump();

	void PrimaryInteract();

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

	void PrimaryAttack();

	void PrimaryAttack_TimeElapsed();

	void BlackHoleAttack();

	void BlackHoleAttack_TimeElapsed();

	void Dash();

	void Dash_TimeElapsed();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};

