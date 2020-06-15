// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Agpaoa_Shader_Stuff.h"

#define LOCTEXT_NAMESPACE "FAgpaoa_Shader_StuffModule"

void FAgpaoa_Shader_StuffModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FAgpaoa_Shader_StuffModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAgpaoa_Shader_StuffModule, Agpaoa_Shader_Stuff)