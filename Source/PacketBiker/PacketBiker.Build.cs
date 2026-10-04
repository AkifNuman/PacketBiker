using UnrealBuildTool;

public class PacketBiker : ModuleRules
{
	public PacketBiker(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore",
			"ChaosVehicles", "EnhancedInput", "UMG"
		});
	}
}
