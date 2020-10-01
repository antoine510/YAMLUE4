#include "YAMLUE4.h"
#include <CoreTypes.h>
#include <Misc/Paths.h>
#include <Internationalization/Text.h>
#include <Modules/ModuleManager.h>
#include <HAL/PlatformProcess.h>
#include <Misc/MessageDialog.h>

bool YAMLUE4Module::initialized = false;
void* YAMLUE4Module::dllHandle = nullptr;

namespace YAMLUE {
	extern void LogMessage(const char* msg);
}

void YAMLUE4Module::StartupModule() {
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Add on the relative location of the third party dll and load it
	FString libraryPath = FPaths::Combine(FPaths::ProjectPluginsDir(), L"YAMLUE4/ThirdParty/yaml-cpp/lib/");
	FString libraryName;

#if PLATFORM_WINDOWS
#if PLATFORM_64BITS
	libraryPath = FPaths::Combine(libraryPath, L"Win64");
#elif PLATFORM_32BITS
	libraryPath = FPaths::Combine(libraryPath, L"Win32");
#endif
	libraryName = L"yaml-cpp.dll";
#elif PLATFORM_LINUX
	libraryPath = FPaths::Combine(libraryPath, L"Linux");
	libraryName = L"libyaml-cpp.so";
#elif PLATFORM_MAC
	libraryPath = FPaths::Combine(libraryPath, L"MacOS");
	libraryName = L"libyaml-cpp.dylib";
#endif
	if(libraryName.Len() == 0) {
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(L"Unsuported platform!"));
		return;
	}

	if(!FPaths::FileExists(FPaths::Combine(libraryPath, libraryName))) {
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(L"Couldn't find YAML-cpp at: " + libraryPath));
		return;
	}

	FPlatformProcess::PushDllDirectory(*libraryPath);

	dllHandle = FPlatformProcess::GetDllHandle(*libraryName);

	FPlatformProcess::PopDllDirectory(*libraryPath);

	if(dllHandle == nullptr) {
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(L"Couldn't load YAML-cpp library"));
		ShutdownModule();
		return;
	}

	initialized = true;
}

void YAMLUE4Module::ShutdownModule() {
	initialized = false;

	FPlatformProcess::FreeDllHandle(dllHandle);
}

IMPLEMENT_MODULE(YAMLUE4Module, YAMLUE4);
