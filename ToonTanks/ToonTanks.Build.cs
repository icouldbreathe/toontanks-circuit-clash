// GPL-2.0 license

using UnrealBuildTool;

public class ToonTanks : ModuleRules
{
	public ToonTanks(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara", "MoviePlayer", "UMG", "RHI",
			"HTTP", "Json"
		});

		//PrivateDependencyModuleNames.AddRange(new[] { });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}