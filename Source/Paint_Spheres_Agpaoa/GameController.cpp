// Fill out your copyright notice in the Description page of Project Settings.


#include "GameController.h"

// Sets default values
AGameController::AGameController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	time = 0.0f;
	targetsexist = true;
}

// Called when the game starts or when spawned
void AGameController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (targetsexist)
	{
		time += DeltaTime;
		FString conversion = "time elapsed: " + FString::SanitizeFloat(time);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, *conversion);
	}
}

