// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Paint_Spheres_AgpaoaProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"

APaint_Spheres_AgpaoaProjectile::APaint_Spheres_AgpaoaProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &APaint_Spheres_AgpaoaProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	//default color and decal goes here
	static ConstructorHelpers::FObjectFinder <UMaterial> asset(TEXT("Material'/Game/Shader/black.black'"));
	paintmat = asset.Object;
	size.X = 150.0f;
	size.Y = 150.0f;
	size.Z = 150.0f;
	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

//isn't polymorphism great? now i can h ave a default projectile and a projectile that's been passed in a value to do different colors
APaint_Spheres_AgpaoaProjectile::APaint_Spheres_AgpaoaProjectile(int color)
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &APaint_Spheres_AgpaoaProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	//here we add a line about throwing a different int into here will output a different color material projectile, we also make set the decal here

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void APaint_Spheres_AgpaoaProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//// Only add impulse and destroy projectile if we hit a physics
	//if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	//{
	//	OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

	//	Destroy();
	//}

	UDecalComponent* SplatDecal = UGameplayStatics::SpawnDecalAttached(paintmat, size, 
		Hit.Component.Get(), Hit.BoneName, Hit.Location, Hit.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 60.0f);



	//destroys this object when it collides with something
	Destroy();
}