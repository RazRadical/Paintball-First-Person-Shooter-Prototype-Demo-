// Copyright Epic Games, Inc. All Rights Reserved.

#include "PaintBallRazWillrichProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Additional libraries:
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"

APaintBallRazWillrichProjectile::APaintBallRazWillrichProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &APaintBallRazWillrichProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Specifying the 'PaintSplat' Material and setting it:
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Splat/PaintSplat_MAT.PaintSplat_MAT'"));

	if (Material.Object != NULL)
	{
		DecalMat = (UMaterial*)Material.Object;
	}

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void APaintBallRazWillrichProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	// Applying the 'PaintSplat' Material onto a surface when hit:
	if (OtherActor != NULL)
	{
		if (DecalMat != nullptr)
		{
			auto Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMat, FVector(UKismetMathLibrary::RandomFloatInRange(20.f, 40.f)), Hit.Location, Hit.Normal.Rotation(), 1.0f);

			auto MatInstance = Decal->CreateDynamicMaterialInstance();

			MatInstance->SetScalarParameterValue("Frame", UKismetMathLibrary::RandomIntegerInRange(0, 3));
			MatInstance->SetVectorParameterValue("Color", FLinearColor(UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f), UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f), UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f)));

			Destroy();
		}
	}
	
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}