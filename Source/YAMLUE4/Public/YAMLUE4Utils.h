#pragma once

extern void YAMLUE::LogMessage(const char* msg);

template <typename T>
bool IsValidT(const T* Test) {
	return ::IsValid(Test) && Test->IsValid();
}

