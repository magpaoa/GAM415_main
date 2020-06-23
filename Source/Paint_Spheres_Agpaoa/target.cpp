// Fill out your copyright notice in the Description page of Project Settings.


#include "target.h"

// Sets default values
Atarget::Atarget()
{
	mesh = CreateDefaultSubobject  <UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void Atarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Atarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void Atarget::OnHit()
{

	this->Destroy();
}