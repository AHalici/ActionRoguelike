// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SCharacter.h"


// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// "SpringArmComp" is just the name we will see in the editor
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	// Root Component = the first component in the hierarchy of the components. In this case, the root would be the capsule
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::MoveForward(float Value)
{
	// What direction we want to move
	AddMovementInput(GetActorForwardVector(), Value);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// BindAxis ranges from 1 to -1. Fully forward is 1 and fully backwards is -1
	// Name of action, whoever owns this input, the function we want to have triggered when we have the key pressed
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);

	// This will let us turn left and right with the mouse Yaw input
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
}


