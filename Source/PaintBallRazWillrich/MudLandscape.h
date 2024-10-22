// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MudLandscape.generated.h"

/**
 * 
 */
UCLASS()
class PAINTBALLRAZWILLRICH_API AMudLandscape : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMudLandscape();

	// A custom function for creating our RenderTarget:
	void renderTargetFunc();

	// A custom function for drawing our Material:
	UFUNCTION(BlueprintCallable, Category = "Mud")
		void GroundHit(FVector2D UVLoc, float Rot);

	// Initial setup of Components, Materials, and RenderTarget:
	UPROPERTY()
		UStaticMeshComponent* landMesh;

	UPROPERTY()
		UMaterialInterface* MudInterface;

	UPROPERTY()
		UMaterialInstanceDynamic* MudDMI;

	UPROPERTY()
		UMaterialInterface* ImpactInterface;

	UPROPERTY()
		UMaterialInstanceDynamic* ImpactDMI;

	UPROPERTY()
		UTextureRenderTarget2D* LandRT;

	// Public variables that're adjustable in the editor:
	UPROPERTY(EditAnywhere)
		float strength = 3.0f;

	UPROPERTY(EditAnywhere)
		float UpMulti = 0.005f;

	UPROPERTY(EditAnywhere)
		float Scale = 0.008f;

	// Variables that're used to calculate position from the HitUV andScale:
	UPROPERTY()
		FVector2D scrnPos;

	UPROPERTY()
		FVector2D scrnSize;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	
};
