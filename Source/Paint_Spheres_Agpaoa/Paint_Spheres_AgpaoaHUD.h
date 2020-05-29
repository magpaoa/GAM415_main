// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Paint_Spheres_AgpaoaHUD.generated.h"

UCLASS()
class APaint_Spheres_AgpaoaHUD : public AHUD
{
	GENERATED_BODY()

public:
	APaint_Spheres_AgpaoaHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

