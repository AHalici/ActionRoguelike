// Fill out your copyright notice in the Description page of Project Settings.

#include "SDashProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"



ASDashProjectile::ASDashProjectile()
{
	DashEffectComp = CreateDefaultSubobject<UParticleSystemComponent>("DashEffectComp");
	DashEffectComp->SetupAttachment(RootComponent);
	
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	MoveComp->InitialSpeed = 6000.0f;
}


void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Sets a timer with a very short fuse (DetonateDelay = 0.2 seconds)
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASDashProjectile::Explode, DetonateDelay);
}


void ASDashProjectile::Explode_Implementation()
{
	// Clear Timer if the Explode() was already called through another source like OnActorHit
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();

	MoveComp->StopMovementImmediately(); // Stop movement so we teleport accurately
	SetActorEnableCollision(false); // Disable collision

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);

	// Skip base implementation as it will destroy actor (we need to stay alive a bit longer to finish the 2nd timer)
	//Super:Explode_Implementation();
}

void ASDashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Actor location before teleporting: %s"), *ActorToTeleport->GetActorLocation().ToString());
		// Keep instigator rotation or it may end up jarring
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
		if (ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false))
		{
			UE_LOG(LogTemp, Warning, TEXT("Teleport SUCCESS <---------------------"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Teleport FAILED <---------------------"));
		}
		//UE_LOG(LogTemp, Warning, TEXT("Actor location after teleporting: %s"), *ActorToTeleport->GetActorLocation().ToString());
	}
}



