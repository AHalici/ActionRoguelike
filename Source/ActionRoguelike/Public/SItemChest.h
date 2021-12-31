// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface // In order to add the interface, we add the ISGameplayInterface
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
	float TargetPitch;
	
	// We add the _Implementation part because it's a BlueprintNativeEvent, otherwise it would be Interact() just like the method is written in the SGameplayInterface header
	void Interact_Implementation(APawn* InstigatorPawn);

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) // BlueprintReadOnly allows us to use this in the event graph of the treasure chest blueprint
	UStaticMeshComponent* LidMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	ASItemChest();
};
