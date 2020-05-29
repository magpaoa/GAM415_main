// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshActor.h"
#include "RMCMeshActor.generated.h"

/**
 * 
 */
UCLASS()
class PAINT_SPHERES_AGPAOA_API ARMCMeshActor : public ARuntimeMeshActor
{
	GENERATED_BODY()
	
public:
	UMaterialInterface* Material;

public:
	ARMCMeshActor();

	void OnConstruction(const FTransform& Transform) override;

private:
	virtual void GenerateBoxMesh();
	virtual void CreateBoxMesh(FVector BoxRadius,
		TArray<FVector>& Vertices,
		TArray<int32>& Triangles,
		TArray<FVector>& Normals,
		TArray<FVector2D>& UVs,
		TArray<FRuntimeMeshTangent>& Tangents,
		TArray<FColor>& Colors);
};
