// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Including directive for plugin:
#include "ProceduralMeshComponent.h"
#include "MyActor.generated.h"

UCLASS()
class PAINTBALLRAZWILLRICH_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Defining functions and a variable that will create mesh: 
	virtual void PostActorCreated() override;

	virtual void PostLoad() override;

	virtual void CreateSquare();

private:
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent* mesh;
};
