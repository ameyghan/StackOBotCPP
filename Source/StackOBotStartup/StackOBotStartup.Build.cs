

using UnrealBuildTool;

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!

public class StackOBotStartup : ModuleRules
{
	public StackOBotStartup(ReadOnlyTargetRules Target)
		: base(Target)
	{
		PrivatePCHHeaderFile = "Public/StackOBotStartup.h";

		PCHUsage = PCHUsageMode.UseSharedPCHs;

		PrivateIncludePaths.Add("StackOBotStartup/Private");

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"MoviePlayer",
				"Slate",
				"SlateCore",
				"InputCore"
			}
		);
	}
}