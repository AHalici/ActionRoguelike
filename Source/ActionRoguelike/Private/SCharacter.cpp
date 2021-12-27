// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"


// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// "SpringArmComp" is just the name we will see in the editor
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	
	// Uses the rotation of the Pawn to rotate the spring arm
	SpringArmComp->bUsePawnControlRotation = true;
	
	// Root Component = the first component in the hierarchy of the components. In this case, the root would be the capsule
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	// Will rotate the character to whatever direction we are moving towards
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	// Stops the camera from moving to the right and left
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}


// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// BindAxis ranges from 1 to -1. Fully forward is 1 and fully backwards is -1
	/* Function Arguments
		(1) Name of action
		(2) Whoever owns this input
		(3) The function we want to have triggered when we have the key pressed */
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	
	// Same thing but with right and left movement
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	// This will let us turn left and right with the mouse Yaw input
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	
	// This will let us pitch our camera up and down with mouse pitch input
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	/* Function Arguments
		(1) Name of action
		(2) Trigger (key press)
		(3) Our character is the user object
		(4) The function to be called */
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}

void ASCharacter::MoveForward(float Value)
{
	// Storing the rotation of our controller
	FRotator ControlRot = GetControlRotation();
	
	// We don't need the pitch and roll so we are setting them to 0 to be safe
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	// Moves towards the vector our control rotator is facing
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	// Storing the rotation of our controller
	FRotator ControlRot = GetControlRotation();
	
	// We don't need the pitch and roll so we are setting them to 0 to be safe
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// X = Forward (Red)
	// Y = Right (Green)
	// Z = Up (Blue)
	
	// This returns our axis-y which is our right vector
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	// Moves toward the right vector from our controller
	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack()
{
	// Get mesh of our character, then get a socket location(grey in Unreal editor, white are bones (Maya, Blender))
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	
	// Our SpawnTM (Spawn Transform Matrix) is the transform for the projectile we will spawn
	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);
	
	FActorSpawnParameters SpawnParams;
	// Spawn collision handling checks if the actor can move a little to avoid collision when they spawn, but since we are spawning the projectile on our character, we don't want any problems for now
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	/* Function Arguments
		<> AActor is the type we are spawning,
		(1) Exposing an asset as a parameter,
		(2) A transform (Spawn transform matrix),
		(3) Optional spawn parameters
	*/
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}

void ASCharacter::Jump()
{
	Super::Jump();
}


