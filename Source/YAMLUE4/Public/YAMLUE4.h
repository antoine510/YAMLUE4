#pragma once

class YAMLUE4Module : public IModuleInterface {
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static bool isLoaded() { return initialized; }

private:
	static bool initialized;
	static void* dllHandle;
};
