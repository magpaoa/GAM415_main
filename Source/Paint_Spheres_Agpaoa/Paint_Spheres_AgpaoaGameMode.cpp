// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Paint_Spheres_AgpaoaGameMode.h"
#include "Paint_Spheres_AgpaoaHUD.h"
#include "Paint_Spheres_AgpaoaCharacter.h"
#include "UObject/ConstructorHelpers.h"

APaint_Spheres_AgpaoaGameMode::APaint_Spheres_AgpaoaGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = APaint_Spheres_AgpaoaHUD::StaticClass();
}
