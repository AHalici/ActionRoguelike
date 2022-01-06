// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"


ASMagicProjectile::ASMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile"); // We are making our own collision preset
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap); // Ignores collision with the player character
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator()) // Instigator is our player character, so this will stop the projectiles from damaging our player
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass())); // Iterates through the components of OtherActor and finds the first instance of the class we specified, which is USAttributeComponent

		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.0f);

			Destroy();
		}
	}
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


