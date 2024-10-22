// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Additional libraries for FogActor Class:
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "FogActor2.generated.h"

UCLASS()
class PAINTBALLRAZWILLRICH_API AFogActor2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFogActor2();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Set the FogActor size:
	UFUNCTION(BlueprintCallable, Category = "Game")
		void setSize(float s);

	// Get the FogActor size:
	UFUNCTION(BlueprintCallable, Category = "Game")
		float getSize() const;

	// Reveals portion of the FogActor:
	UFUNCTION(BlueprintCallable, Category = "Game")
		void revealSmoothCircle(const FVector2D& pos, float radius);

private:
	// FogActor texture size:
	static const int m_iceTextureSize = 512;
	UPROPERTY()
		UStaticMeshComponent* m_iceSquarePlane;
	UPROPERTY()
		UTexture2D* m_iceDynamicTexture;
	UPROPERTY()
		UMaterialInterface* m_iceDynamicMaterial;
	UPROPERTY()
		UMaterialInstanceDynamic* m_iceDynamicMaterialInstance;

	// Array sized for the texture:
	uint8 m_icePixelArray[m_iceTextureSize * m_iceTextureSize];
	FUpdateTextureRegion2D m_iceWholeTextureRegion;
	float m_iceCoverSize;
};