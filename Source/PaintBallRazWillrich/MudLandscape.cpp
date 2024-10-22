// Fill out your copyright notice in the Description page of Project Settings.


#include "MudLandscape.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Canvas.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

AMudLandscape::AMudLandscape()
{
	// Initializing the Component and setting it as the root:
	landMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Landscape Mesh"));
	RootComponent = landMesh;
	{
		// Retrieving the landscape mesh in project Content and setting it:
		static ConstructorHelpers::FObjectFinder<UStaticMesh> asset(TEXT("StaticMesh'/Game/Mud/landmesh1.landmesh1'"));
		landMesh->SetStaticMesh(asset.Object);
	}

	{
		// Retrieving the mud Material:
		static ConstructorHelpers::FObjectFinder<UMaterial> asset(TEXT("Material'/Game/Mud/Mud_MAT.Mud_MAT'"));
		MudInterface = asset.Object;
	}

	{
		// Retireving the impact Material:
		static ConstructorHelpers::FObjectFinder<UMaterial> asset(TEXT("Material'/Game/Mud/Impact_MAT.Impact_MAT'"));
		ImpactInterface = asset.Object;
	}
}

void AMudLandscape::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// If the mud Material is set, then create a Dynamic Material instance and set it to the landscape mesh:
	if (MudInterface)
	{
		MudDMI = UMaterialInstanceDynamic::Create(MudInterface, this);
		landMesh->SetMaterial(0, MudDMI);
	}

	// If the impact Material is set, then set the default values of our parameters inside the Material:
	if (ImpactInterface)
	{
		ImpactDMI = UMaterialInstanceDynamic::Create(ImpactInterface, this);
		ImpactDMI->SetScalarParameterValue("Strength", strength);
		ImpactDMI->SetScalarParameterValue("UpMulti", UpMulti);
	}
}

void AMudLandscape::BeginPlay()
{
	Super::BeginPlay();

	// Setup RenderTarget:
	renderTargetFunc();
}

void AMudLandscape::renderTargetFunc()
{
	// Creates the RenderTarget and sets it as the TextureParameter inside of our mud Material:
	LandRT = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 8192, 8192, ETextureRenderTargetFormat::RTF_RGBA8, FLinearColor::White, false);
	MudDMI->SetTextureParameterValue("Dis", LandRT);
}

void AMudLandscape::GroundHit(FVector2D UVLoc, float Rot)
{
	// Initial local variables:
	UCanvas* Canvas;
	FVector2D size;
	FDrawToRenderTargetContext Rendercontext;

	// Begin Drawing to canvas:
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), LandRT, Canvas, size, Rendercontext);

	// Calculate location using the HitUV's and Scale:
	scrnPos = (size * (UVLoc - 0.5f)) + ((size * 0.5f) * (1.0f - Scale));
	scrnSize = (size * Scale);

	// Draw the impact Material:
	Canvas->K2_DrawMaterial(ImpactDMI, scrnPos, scrnSize, FVector2D(0.0f, 0.0f), FVector2D(1.0f, 1.0f), Rot, FVector2D(0.5f, 0.5f));
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Rendercontext);

	// Debug to ensure that the event is being called. Comment this section out to hide it:
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("GroundHit Activated.")));
}