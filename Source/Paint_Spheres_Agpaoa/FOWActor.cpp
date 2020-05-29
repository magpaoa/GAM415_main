// Fill out your copyright notice in the Description page of Project Settings.


#include "FOWActor.h"
#include "Engine/CollisionProfile.h"


/**
 * @brief AFOWActor::AFOWActor
 *
 * Default Constructor
 */
AFOWActor::AFOWActor() : m_wholeTextureRegion(0, 0, 0, 0, m_textureSize, m_textureSize)
{
    UE_LOG(LogTemp, Warning, TEXT("AFOWActor::AFOWActor()"));

    // default size
    m_coverSize = 1000;

    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Create a planar mesh from engine's planar static mesh.
    m_squarePlane = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("FOW Plane Static Mesh"));
    RootComponent = m_squarePlane;
    m_squarePlane->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
    {
        static ConstructorHelpers::FObjectFinder <UStaticMesh> asset(TEXT("/Engine/ArtTools/RenderToTexture/Meshes/S_1_Unit_Plane.S_1_Unit_Plane"));
        m_squarePlane->SetStaticMesh(asset.Object);
    }

    m_squarePlane->SetRelativeScale3D(FVector(m_coverSize, m_coverSize, 1));
    // Load the base material from your created material.
    {
        static ConstructorHelpers::FObjectFinder <UMaterial> asset(TEXT("Material'/Game/Material/FowMat.FowMat'"));
        m_dynamicMaterial = asset.Object;
    }

    // Create the runtime FOW texture.
    if (!m_dynamicTexture)
    {
        m_dynamicTexture = UTexture2D::CreateTransient(m_textureSize, m_textureSize, PF_G8);
        m_dynamicTexture->CompressionSettings = TextureCompressionSettings::TC_Grayscale;
        m_dynamicTexture->SRGB = 0;
        m_dynamicTexture->UpdateResource();
        m_dynamicTexture->MipGenSettings = TMGS_NoMipmaps;
    }

    // Initialize array to opaque (255)
    for (int x = 0; x < m_textureSize; ++x)
        for (int y = 0; y < m_textureSize; ++y)
            m_pixelArray[y * m_textureSize + x] = 255;

    // Propagate memory's array to the texture.
    if (m_dynamicTexture)
        m_dynamicTexture->UpdateTextureRegions(0, 1, &m_wholeTextureRegion, m_textureSize, 1, m_pixelArray);
}


/**
 * @brief AFOWActor::BeginPlay
 *
 * Called when the level is started
 */
void AFOWActor::BeginPlay()
{
    Super::BeginPlay();
}


/**
 * @brief AFOWActor::Tick
 *
 * @param DeltaTime Time in seconds since the last update
 */
void AFOWActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime); // Call parent class tick function.
}


/**
 * @brief AFOWActor::PostInitializeComponents
 *
 * Called after the Actor's components have been initialized
 */
void AFOWActor::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    // Create a dynamic material instance to swap in the FOW texture. The material must have a
    // "FowTexture" parameter
    if (m_dynamicMaterial)
    {
        m_dynamicMaterialInstance = UMaterialInstanceDynamic::Create(m_dynamicMaterial, this);
        m_dynamicMaterialInstance->SetTextureParameterValue("FowTexture", m_dynamicTexture);
    }

    // Set the dynamic material to the mesh (only if memory changed)
    if (m_dynamicMaterialInstance)
        m_squarePlane->SetMaterial(0, m_dynamicMaterialInstance);
}


/**
 * @brief AFOWActor::setSize
 *
 * @param s Lenght of the side of the square in Unreal units
 */
void AFOWActor::setSize(float s)
{
    m_coverSize = s;
    m_squarePlane->SetRelativeScale3D(FVector(m_coverSize, m_coverSize, 1));
}


/**
 * @brief AFOWActor::getSize
 *
 * Getter function
 *
 * @return Return plane side length
 */
float AFOWActor::getSize() const
{
    return m_coverSize;
}


/**
 * @brief AFOWActor::revealSmoothCircle
 *
 * It makes the circle (centered at pos) transparent -- with smooth boundaries. It then calls
 * the texture update function
 *
 * @param pos Center of the circle in world coordinates
 * @param radius Radius of the circle in world units
 */
void AFOWActor::revealSmoothCircle(const FVector2D& pos, float radius)
{
    // Calculate the where circle center is inside texture space
    const FVector location = GetActorLocation();
    // Coordinates in world units respect to the location of the actor
    FVector2D texel = pos - FVector2D(location.X, location.Y);
    // Convert these coordinates to texture units
    texel = texel * m_textureSize / m_coverSize;
    // (0,0) in UV coordinates is at the bottom-left of the texture
    texel += FVector2D(m_textureSize / 2, m_textureSize / 2);

    // Calculate radius in texture units ( 1 is 1 pixel )
    const float texelRadius = radius * m_textureSize / m_coverSize;

    // The square area to update
    const int minX = FMath::Clamp <int>(texel.X - texelRadius, 0, m_textureSize - 1);
    const int minY = FMath::Clamp <int>(texel.Y - texelRadius, 0, m_textureSize - 1);
    const int maxX = FMath::Clamp <int>(texel.X + texelRadius, 0, m_textureSize - 1);
    const int maxY = FMath::Clamp <int>(texel.Y + texelRadius, 0, m_textureSize - 1);

    // Update our array of FOW value in memory
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

    // Propagate the values in memory's array to the texture (only if memory changed)
    if (dirty)
    {
        // We update only the region of the texture that has changed
        m_wholeTextureRegion.DestX = minX;
        m_wholeTextureRegion.DestY = minY;
        m_wholeTextureRegion.SrcX = minX;
        m_wholeTextureRegion.SrcY = minY;
        m_wholeTextureRegion.Width = maxX - minX + 1;
        m_wholeTextureRegion.Height = maxY - minY + 1;

        m_dynamicTexture->UpdateTextureRegions(0, 1, &m_wholeTextureRegion, m_textureSize, 1, m_pixelArray);
    }
}