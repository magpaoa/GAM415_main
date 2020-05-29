// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "FOWActor.generated.h"

UCLASS()
class PAINT_SPHERES_AGPAOA_API AFOWActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AFOWActor();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void PostInitializeComponents() override;

public:
    // Set the plane's size
    UFUNCTION(BlueprintCallable, Category = "Game")
        void setSize(float s);
    // Set the plane's size
        UFUNCTION(BlueprintCallable, Category = "Game")
        float getSize() const;

    // Reveal (make transparent) a portion of the plane
        UFUNCTION(BlueprintCallable, Category = "Game")
        void revealSmoothCircle(const FVector2D& pos, float radius);

private:
    // FOW texture size
    static const int m_textureSize = 512;
    UPROPERTY()
        UStaticMeshComponent* m_squarePlane;
    UPROPERTY()
        UTexture2D* m_dynamicTexture;
    UPROPERTY()
        UMaterialInterface* m_dynamicMaterial;
    UPROPERTY()
        UMaterialInstanceDynamic* m_dynamicMaterialInstance;

    uint8 m_pixelArray[m_textureSize * m_textureSize]; // Array the size of the texture
    FUpdateTextureRegion2D m_wholeTextureRegion;

    float m_coverSize;
};