// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "CubeActor.generated.h"

UCLASS()
class PAINT_SPHERES_AGPAOA_API ACubeActor : public AActor
{
	GENERATED_BODY()
	
public:
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;
	virtual void GenerateBoxMesh();
	virtual void CreateBoxMesh(FVector BoxRadius, TArray <FVector>& Vertices, TArray <int32>& Triangles, TArray <FVector>& Normals, TArray <FVector2D>& UVs, TArray <FProcMeshTangent>& Tangents, TArray <FColor>& Colors);

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* mesh;

public:	
	// Sets default values for this actor's properties
	ACubeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
