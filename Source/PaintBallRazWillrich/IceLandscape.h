// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "IceLandscape.generated.h"

/**
 * 
 */
UCLASS()
class PAINTBALLRAZWILLRICH_API AIceLandscape : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AIceLandscape();

	// A custom function for creating our RenderTarget:
	void IceRenderTargetFunc();

	// A custom function for drawing our Material:
	UFUNCTION(BlueprintCallable, Category = "Ice")
		void GroundHit(FVector2D UVLoc, float Rot);

	// Initial setup of Components, Materials, and RenderTarget:
	UPROPERTY()
		UStaticMeshComponent* IcelandMesh;

	UPROPERTY()
		UMaterialInterface* SnowInterface;

	UPROPERTY()
		UMaterialInstanceDynamic* SnowDMI;

	UPROPERTY()
		UMaterialInterface* IceImpactInterface;

	UPROPERTY()
		UMaterialInstanceDynamic* IceImpactDMI;

	UPROPERTY()
		UTextureRenderTarget2D* IcelandRT;

	// Public variables that're adjustable in the editor:
	UPROPERTY(EditAnywhere)
		float strength = 3.0f;

	UPROPERTY(EditAnywhere)
		float UpMulti = 0.005f;

	UPROPERTY(EditAnywhere)
		float Scale = 0.05f;

	// Variables that're used to calculate position from the HitUV andScale:
	UPROPERTY()
		FVector2D scrnPos;

	UPROPERTY()
		FVector2D scrnSize;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	
};
