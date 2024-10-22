// Fill out your copyright notice in the Description page of Project Settings.


#include "IceLandscape.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Canvas.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

AIceLandscape::AIceLandscape()
{
	// Initializing the Component and setting it as the root:
	IcelandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Landscape Mesh"));
	RootComponent = IcelandMesh;
	{
		// Retrieving the landscape mesh in project Content and setting it:
		static ConstructorHelpers::FObjectFinder<UStaticMesh> asset(TEXT("StaticMesh'/Game/Mud/landmesh2.landmesh2'"));
		IcelandMesh->SetStaticMesh(asset.Object);
	}

	{
		// Retrieving the snow Material:
		static ConstructorHelpers::FObjectFinder<UMaterial> asset(TEXT("Material'/Game/Mud/Snow_MAT.Snow_MAT'"));
		SnowInterface = asset.Object;
	}

	{
		// Retireving the impact Material:
		static ConstructorHelpers::FObjectFinder<UMaterial> asset(TEXT("Material'/Game/Mud/Impact_MAT.Impact_MAT'"));
		IceImpactInterface = asset.Object;
	}
}

void AIceLandscape::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// If the snow Material is set, then create a Dynamic Material instance and set it to the landscape mesh:
	if (SnowInterface)
	{
		SnowDMI = UMaterialInstanceDynamic::Create(SnowInterface, this);
		IcelandMesh->SetMaterial(0, SnowDMI);
	}

	// If the impact Material is set, then set the default values of our parameters inside the Material:
	if (IceImpactInterface)
	{
		IceImpactDMI = UMaterialInstanceDynamic::Create(IceImpactInterface, this);
		IceImpactDMI->SetScalarParameterValue("Strength", strength);
		IceImpactDMI->SetScalarParameterValue("UpMulti", UpMulti);
	}
}

void AIceLandscape::BeginPlay()
{
	Super::BeginPlay();

	// Setup RenderTarget:
	IceRenderTargetFunc();
}

void AIceLandscape::IceRenderTargetFunc()
{
	// Creates the RenderTarget and sets it as the TextureParameter inside of our snow Material:
	IcelandRT = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 8192, 8192, ETextureRenderTargetFormat::RTF_RGBA8, FLinearColor::White, false);
	SnowDMI->SetTextureParameterValue("Displacement", IcelandRT);
}

void AIceLandscape::GroundHit(FVector2D UVLoc, float Rot)
{
	// Initial local variables:
	UCanvas* Canvas;
	FVector2D size;
	FDrawToRenderTargetContext Rendercontext;

	// Begin Drawing to canvas:
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), IcelandRT, Canvas, size, Rendercontext);

	// Calculate location using the HitUV's and Scale:
	scrnPos = (size * (UVLoc - 0.5f)) + ((size * 0.5f) * (1.0f - Scale));
	scrnSize = (size * Scale);

	// Draw the impact Material:
	Canvas->K2_DrawMaterial(IceImpactDMI, scrnPos, scrnSize, FVector2D(0.0f, 0.0f), FVector2D(1.0f, 1.0f), Rot, FVector2D(0.5f, 0.5f));
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Rendercontext);

	// Debug to ensure that the event is being called. Comment this section out to hide it:
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("GroundHit Activated.")));
}