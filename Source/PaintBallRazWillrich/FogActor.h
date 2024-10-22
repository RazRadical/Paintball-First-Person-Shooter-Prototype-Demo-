// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Additional libraries for FogActor Class:
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "FogActor.generated.h"

UCLASS()
class PAINTBALLRAZWILLRICH_API AFogActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFogActor();

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
	static const int m_textureSize = 512;
	UPROPERTY() 
		UStaticMeshComponent* m_squarePlane;
	UPROPERTY() 
		UTexture2D* m_dynamicTexture;
	UPROPERTY() 
		UMaterialInterface* m_dynamicMaterial;
	UPROPERTY() 
		UMaterialInstanceDynamic* m_dynamicMaterialInstance;

	// Array sized for the texture:
	uint8 m_pixelArray[m_textureSize * m_textureSize];
	FUpdateTextureRegion2D m_wholeTextureRegion;
	float m_coverSize;
};
