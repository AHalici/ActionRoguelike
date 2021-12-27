// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"

// Sets default values for this component's properties

USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts

void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner(); // Will be our character in this case

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000); // Extending the end of the line trace in the direction we are looking by 1000 units (1000 cm)
	
	//FHitResult Hit;
	// Will start somewhere in the world, and go somewhere else in the world and try to figure out if there is some type of object to collide with in between (basically raycast)
	/* Function Arguments
		(1) Hit result information
		(2) Start of line trace
		(3) End of line trace
		(4) Query Parameters
	 */

	// Boolean to hold result of if line tracing hit correct object
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	TArray<FHitResult> Hits;
	
	float Radius = 30.0f; // For DrawDebugSphere
	
	FCollisionShape Shape;
	Shape.SetSphere(30.0f);

	// A sweep is when in physics, a sphere is projected from start point to end point to see overlaps and to see where it's first blocking hit is on it's radius
		// FQuat::Identity is a quaternion and default rotation / no rotation
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		// We need to make sure the item we hit with the line trace is not null and is the treasure chest and not just some other WorldDynamic object
		if (HitActor)
		{
			if (HitActor->Implements<USGameplayInterface>())
			// If we want to check if it implements it or cast it, we have to use the USGameplayInterface ('U' prefix), but when we call on it, we use ISGameplayInterface ('I' prefix)
			{
				APawn* MyPawn = Cast<APawn>(MyOwner); // Casting our owner from an AActor to an APawn

				/* Function Arguments
				(1) The object we want to call Execute_Interact (Interact()) on
				(2) The owner/instigator
			 */
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
	}

	
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
}

