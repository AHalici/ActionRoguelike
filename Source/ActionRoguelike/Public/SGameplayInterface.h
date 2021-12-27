// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGameplayInterface.generated.h"

// This class does not need to be modified.

UINTERFACE(MinimalAPI)
class USGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

// We use this class definition and not the one above
	// The 'I' prefix denotes that it is an interface class
class ACTIONROGUELIKE_API ISGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/* Function Parameters
		(1) The pawn that triggered the interact (ex: Who used the health potion)
	*/
	UFUNCTION(BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);
};

