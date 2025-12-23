// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MultiPlayer_AmongUs : ModuleRules
{
	public MultiPlayer_AmongUs(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
            "OnlineSubsystem",
            "OnlineSubsystemUtils"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"MultiPlayer_AmongUs",
			"MultiPlayer_AmongUs/Variant_Platforming",
			"MultiPlayer_AmongUs/Variant_Platforming/Animation",
			"MultiPlayer_AmongUs/Variant_Combat",
			"MultiPlayer_AmongUs/Variant_Combat/AI",
			"MultiPlayer_AmongUs/Variant_Combat/Animation",
			"MultiPlayer_AmongUs/Variant_Combat/Gameplay",
			"MultiPlayer_AmongUs/Variant_Combat/Interfaces",
			"MultiPlayer_AmongUs/Variant_Combat/UI",
			"MultiPlayer_AmongUs/Variant_SideScrolling",
			"MultiPlayer_AmongUs/Variant_SideScrolling/AI",
			"MultiPlayer_AmongUs/Variant_SideScrolling/Gameplay",
			"MultiPlayer_AmongUs/Variant_SideScrolling/Interfaces",
			"MultiPlayer_AmongUs/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
