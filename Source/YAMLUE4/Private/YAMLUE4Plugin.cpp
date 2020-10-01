#include "YAMLUE4Plugin.h"

std::string YAML::detail::node_data::empty_scalar;

FLogCategoryYAMLLog YAMLLog;

void YAMLUE::LogMessage(const FString& msg) {
	UE_LOG(YAMLLog, Log, L"%s", *msg);
}

void YAMLUE::LogMessageOnScreen(const FString& msg) {
	LogMessage(msg);
	if(GEngine != nullptr) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, msg);
}

void YAMLUE::LogMessage(const char* msg) {
	UE_LOG(YAMLLog, Log, L"%s", UTF8_TO_TCHAR(msg));
}

void YAMLUE::LogMessageOnScreen(const char* msg) {
	LogMessage(msg);
	if(GEngine != nullptr) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, UTF8_TO_TCHAR(msg));
}

void YAMLUE::LogMessage(const std::string& msg) {
	UE_LOG(YAMLLog, Log, L"%s", UTF8_TO_TCHAR(msg.c_str()));
}

void YAMLUE::LogMessageOnScreen(const std::string& msg) {
	LogMessage(msg);
	if(GEngine != nullptr) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, UTF8_TO_TCHAR(msg.c_str()));
}

