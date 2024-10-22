// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PaintBallRazWillrich : ModuleRules
{
	public PaintBallRazWillrich(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "ProceduralMeshComponent", "RuntimeMeshComponent", "RenderCore" });
	}
}
