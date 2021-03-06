#pragma once

#include <CoreTypes.h>
#include <string>

#include "yaml-cpp/yaml.h"

#include "YAMLUE4.h"


extern YAMLUE4_API struct FLogCategoryYAMLLog : public FLogCategory<ELogVerbosity::Log, ELogVerbosity::All> {
	FLogCategoryYAMLLog() : FLogCategory(TEXT("YAML Log")) {}
} YAMLLog;

namespace YAMLUE {

void YAMLUE4_API LogMessage(const FString& msg);
void YAMLUE4_API LogMessageOnScreen(const FString& msg);
void YAMLUE4_API LogMessage(const char* msg);
void YAMLUE4_API LogMessageOnScreen(const char* msg);
void YAMLUE4_API LogMessage(const std::string& msg);
void YAMLUE4_API LogMessageOnScreen(const std::string& msg);

}
