// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Cours_Unreal_CPP : ModuleRules
{
	public Cours_Unreal_CPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
