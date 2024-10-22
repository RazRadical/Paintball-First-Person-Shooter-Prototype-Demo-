// Fill out your copyright notice in the Description page of Project Settings.


#include "FogActor2.h"

// Additional libraries for FogActor Class:
#include "Engine/CollisionProfile.h"

// Sets default values
AFogActor2::AFogActor2() :m_iceWholeTextureRegion(0, 0, 0, 0, m_iceTextureSize, m_iceTextureSize)
{
	UE_LOG(LogTemp, Warning, TEXT("AFogActor2::AFogActor2()"))

	// Default size:
	m_iceCoverSize = 1000;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a planar mesh from engine's planar static mesh:
	m_iceSquarePlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fog Plane Static Mesh"));
	RootComponent = m_iceSquarePlane;
	m_iceSquarePlane->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> asset(TEXT("Class'/Script/PaintBallRazWillrich.IceLandscape'"));
		m_iceSquarePlane->SetStaticMesh(asset.Object);
	}

	m_iceSquarePlane->SetRelativeScale3D(FVector(m_iceCoverSize, m_iceCoverSize, 1));

	// Load the base material from your created material:
	{
		static ConstructorHelpers::FObjectFinder<UMaterial> asset(TEXT("Material'/Game/Mud/FogActor2/SnowFog_MAT.SnowFog_MAT'"));
		m_iceDynamicMaterial = asset.Object;
	}

	// Create the runtime FogActor texture:
	if (!m_iceDynamicTexture)
	{
		m_iceDynamicTexture = UTexture2D::CreateTransient(m_iceTextureSize, m_iceTextureSize, PF_G8);
		m_iceDynamicTexture->CompressionSettings = TextureCompressionSettings::TC_Grayscale;
		m_iceDynamicTexture->SRGB = 0;
		m_iceDynamicTexture->UpdateResource();
		m_iceDynamicTexture->MipGenSettings = TMGS_NoMipmaps;
	}

	// Initialize array to all black:
	for (int x = 0; x < m_iceTextureSize; ++x)
		for (int y = 0; y < m_iceTextureSize; ++y)
			m_icePixelArray[y * m_iceTextureSize + x] = 255;

	// Propagate memory's array to the texture:
	if (m_iceDynamicTexture)
		m_iceDynamicTexture->UpdateTextureRegions(0, 1, &m_iceWholeTextureRegion, m_iceTextureSize, 1, m_icePixelArray);
}

// Called when the game starts or when spawned
void AFogActor2::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFogActor2::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Create a dynamic material instance to swap in the FogActor texture:
	if (m_iceDynamicMaterial)
	{
		m_iceDynamicMaterialInstance = UMaterialInstanceDynamic::Create(m_iceDynamicMaterial, this);
		m_iceDynamicMaterialInstance->SetTextureParameterValue("SnowFogTexture", m_iceDynamicTexture);
	}

	// Set the dynamic material to the mesh:
	if (m_iceDynamicMaterialInstance)
		m_iceSquarePlane->SetMaterial(0, m_iceDynamicMaterialInstance);

}

void AFogActor2::setSize(float s)
{
	m_iceCoverSize = s;
	m_iceSquarePlane->SetRelativeScale3D(FVector(m_iceCoverSize, m_iceCoverSize, 1));
}

float AFogActor2::getSize() const
{
	return m_iceCoverSize;
}

void AFogActor2::revealSmoothCircle(const FVector2D& pos, float radius)
{
	// Calculate the circle's center inside of texture space:
	const FVector location = GetActorLocation();
	FVector2D texel = pos - FVector2D(location.X, location.Y);
	texel = texel * m_iceTextureSize / m_iceCoverSize;
	texel += FVector2D(m_iceTextureSize / 2, m_iceTextureSize / 2);

	// Calculate radius in texel unit (1 x 1 pixel):
	const float texelRadius = radius * m_iceTextureSize / m_iceCoverSize;

	// The square area to update:
	int minX = FMath::Clamp <int>(texel.X - texelRadius, 0, m_iceTextureSize - 1);
	int minY = FMath::Clamp <int>(texel.Y - texelRadius, 0, m_iceTextureSize - 1);
	int maxX = FMath::Clamp <int>(texel.X + texelRadius, 0, m_iceTextureSize - 1);
	int maxY = FMath::Clamp <int>(texel.Y + texelRadius, 0, m_iceTextureSize - 1);

	bool dirty = false;
	for (int x = minX; x <= maxX; ++x)
	{
		for (int y = minY; y <= maxY; ++y)
		{
			float distance = FVector2D::Distance(texel, FVector2D(x, y));

			if (distance < texelRadius)
			{
				static float smoothPct = 0.7f;
				uint8 oldVal = m_icePixelArray[y * m_iceTextureSize + x];
				float lerp = FMath::GetMappedRangeValueClamped(FVector2D(smoothPct, 1.0f), FVector2D(0, 1), distance / texelRadius);
				uint8 newVal = lerp * 255;
				newVal = FMath::Min(newVal, oldVal);
				m_icePixelArray[y * m_iceTextureSize + x] = newVal;
				dirty = dirty || oldVal != newVal;
			}
		}
	}

	// Propagate the values in memory's array to texture:
	if (dirty)
	{
		m_iceWholeTextureRegion.DestX = minX;
		m_iceWholeTextureRegion.DestY = minY;
		m_iceWholeTextureRegion.SrcX = minX;
		m_iceWholeTextureRegion.SrcY = minY;
		m_iceWholeTextureRegion.Width = maxX - minX + 1;
		m_iceWholeTextureRegion.Height = maxY - minY + 1;

		m_iceDynamicTexture->UpdateTextureRegions(0, 1, &m_iceWholeTextureRegion, m_iceTextureSize, 1, m_icePixelArray);
	}
}

// Called every frame
void AFogActor2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

