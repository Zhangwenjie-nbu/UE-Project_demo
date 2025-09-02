using UnrealBuildTool;

public class QTLoadingScreen : ModuleRules
{
	public QTLoadingScreen(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivatePCHHeaderFile = "Public/QTLoadingScreen.h";
		PrivateIncludePaths.Add("QTLoadingScreen/Private");
		PCHUsage = PCHUsageMode.UseSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			 "Core", "CoreUObject", "Engine"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			 "MoviePlayer",
			 "SlateCore",
			 "UMG"
		});
	}
}