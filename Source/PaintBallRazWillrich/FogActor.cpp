// Fill out your copyright notice in the Description page of Project Settings.


#include "FogActor.h"

// Additional libraries for FogActor Class:
#include "Engine/CollisionProfile.h"

// Sets default values
AFogActor::AFogActor() :m_wholeTextureRegion(0, 0, 0, 0, m_textureSize, m_textureSize)
{
	UE_LOG(LogTemp, Warning, TEXT("AFogActor::AFogActor()"))

	// Default size:
	m_coverSize = 1000;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a planar mesh from engine's planar static mesh:
	m_squarePlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fog Plane Static Mesh"));
	RootComponent = m_squarePlane;
	m_squarePlane->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> asset(TEXT("StaticMesh'/Game/Mud/landmesh2.landmesh2'"));
		m_squarePlane->SetStaticMesh(asset.Object);
	}

	m_squarePlane->SetRelativeScale3D(FVector(m_coverSize, m_coverSize, 1));

	// Load the base material from your created material:
	{
		static ConstructorHelpers::FObjectFinder<UMaterial> asset(TEXT("Material'/Game/Shaders/Fog_MAT.Fog_MAT'"));
		m_dynamicMaterial = asset.Object;
	}

	// Create the runtime FogActor texture:
	if (!m_dynamicTexture)
	{
		m_dynamicTexture = UTexture2D::CreateTransient(m_textureSize, m_textureSize, PF_G8);
		m_dynamicTexture->CompressionSettings = TextureCompressionSettings::TC_Grayscale;
		m_dynamicTexture->SRGB = 0;
		m_dynamicTexture->UpdateResource();
		m_dynamicTexture->MipGenSettings = TMGS_NoMipmaps;
	}

	// Initialize array to all black:
	for (int x = 0; x < m_textureSize; ++x)
		for (int y = 0; y < m_textureSize; ++y)
			m_pixelArray[y * m_textureSize + x] = 255;

	// Propagate memory's array to the texture:
	if (m_dynamicTexture)
		m_dynamicTexture->UpdateTextureRegions(0, 1, &m_wholeTextureRegion, m_textureSize, 1, m_pixelArray);
}

// Called when the game starts or when spawned
void AFogActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFogActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Create a dynamic material instance to swap in the FogActor texture:
	if (m_dynamicMaterial)
	{
		m_dynamicMaterialInstance = UMaterialInstanceDynamic::Create(m_dynamicMaterial, this);
		m_dynamicMaterialInstance->SetTextureParameterValue("FogTexture", m_dynamicTexture);
	}

	// Set the dynamic material to the mesh:
	if (m_dynamicMaterialInstance)
		m_squarePlane->SetMaterial(0, m_dynamicMaterialInstance);

}

void AFogActor::setSize(float s)
{
	m_coverSize = s;
	m_squarePlane->SetRelativeScale3D(FVector(m_coverSize, m_coverSize, 1));
}

float AFogActor::getSize() const
{
	return m_coverSize;
}

void AFogActor::revealSmoothCircle(const FVector2D& pos, float radius)
{
	// Calculate the circle's center inside of texture space:
	const FVector location = GetActorLocation();
	FVector2D texel = pos - FVector2D(location.X, location.Y);
	texel = texel * m_textureSize / m_coverSize;
	texel += FVector2D(m_textureSize / 2, m_textureSize / 2);

	// Calculate radius in texel unit (1 x 1 pixel):
	const float texelRadius = radius * m_textureSize / m_coverSize; 

	// The square area to update:
	int minX = FMath::Clamp <int>(texel.X - texelRadius, 0, m_textureSize - 1);
	int minY = FMath::Clamp <int>(texel.Y - texelRadius, 0, m_textureSize - 1);
	int maxX = FMath::Clamp <int>(texel.X + texelRadius, 0, m_textureSize - 1);
	int maxY = FMath::Clamp <int>(texel.Y + texelRadius, 0, m_textureSize - 1);

	bool dirty = false;
	for (int x = minX; x <= maxX; ++x)
	{
		for (int y = minY; y <= maxY; ++y)
		{
			float distance = FVector2D::Distance(texel, FVector2D(x, y));

			if (distance < texelRadius)
			{
				static float smoothPct = 0.7f;
				uint8 oldVal = m_pixelArray[y * m_textureSize + x];
				float lerp = FMath::GetMappedRangeValueClamped(FVector2D(smoothPct, 1.0f), FVector2D(0, 1), distance / texelRadius);
				uint8 newVal = lerp * 255;
				newVal = FMath::Min(newVal, oldVal);
				m_pixelArray[y * m_textureSize + x] = newVal;
				dirty = dirty || oldVal != newVal;
			}
		}
	}

	// Propagate the values in memory's array to texture:
	if (dirty)
	{
		m_wholeTextureRegion.DestX = minX;
		m_wholeTextureRegion.DestY = minY;
		m_wholeTextureRegion.SrcX = minX;
		m_wholeTextureRegion.SrcY = minY;
		m_wholeTextureRegion.Width = maxX - minX + 1;
		m_wholeTextureRegion.Height = maxY - minY + 1;

		m_dynamicTexture->UpdateTextureRegions(0, 1, &m_wholeTextureRegion, m_textureSize, 1, m_pixelArray);
	}
}

// Called every frame
void AFogActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

