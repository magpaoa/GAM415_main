// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Paint_Spheres_AgpaoaEditorTarget : TargetRules
{
	public Paint_Spheres_AgpaoaEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("Paint_Spheres_Agpaoa");
	}
}
