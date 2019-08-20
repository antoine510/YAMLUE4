using UnrealBuildTool;
using System.Collections.Generic;
using System.IO;

public class YAMLUE4 : ModuleRules {

	private string yamlcppPath {
		get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/yaml-cpp/")); }
	}

	public YAMLUE4(ReadOnlyTargetRules Target) : base(Target) {
		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
		PrivatePCHHeaderFile = "Private/YAMLUE4PCH.h";

		if (Target.Platform != UnrealTargetPlatform.Win64) {
			throw new System.Exception("This plugin is only available for Win64 right now.");
		}

		PrivateIncludePaths.Add(Path.Combine(yamlcppPath, "include/"));
		
		PublicLibraryPaths.Add(Path.Combine(yamlcppPath, "lib/Win64/"));
		PublicAdditionalLibraries.Add("yaml-cpp.lib");

		PublicDelayLoadDLLs.Add("yaml-cpp.dll");

		addDependency("Win64", "yaml-cpp.dll");
		
		bEnableExceptions = true;
	}
	
	private void addDependency(string arch, string lib) {
		RuntimeDependencies.Add(Path.Combine(yamlcppPath, "lib/", arch, lib));
	}
}
