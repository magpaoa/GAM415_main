// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Paint_Spheres_AgpaoaTarget : TargetRules
{
	public Paint_Spheres_AgpaoaTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("Paint_Spheres_Agpaoa");
	}
}
