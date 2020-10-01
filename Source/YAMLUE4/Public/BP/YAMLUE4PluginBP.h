#pragma once

#include "YAMLUE4Plugin.h"
#include "YAMLUE4Utils.h"

#include "YAMLUE4PluginBP.generated.h"

using namespace YAMLUE;

UENUM(BlueprintType)
enum class EYAMLErrorState : uint8 {
	Ok,
	Fail
};


UCLASS(BlueprintType)
class YAMLUE4_API UYAMLNode : public UObject {
	GENERATED_UCLASS_BODY()
public:

	void BeginDestroy() override {
		Super::BeginDestroy();
		//delete _node;
	}

	UFUNCTION(BlueprintCallable, Category = "YAML")
		static void LoadDocument(const FString& path, UYAMLNode*& root);

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "YAMLNode|Cast")
		void GetNode(const FString& name, UYAMLNode*& node);

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "YAMLNode|Cast")
		bool GetNodeFallback(const FString& name, const FString& fallback, UYAMLNode*& node);

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "Has", Keywords = "check has", BlueprintAutocast), Category = "YAMLNode|Check")
		bool HasNode(const FString& name) {
		return HasNode(std::string(TCHAR_TO_UTF8(*name)));
	}

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "YAMLNode|Cast")
		void GetNodeArray(const FString& name, TArray<UYAMLNode*>& nodeArray);

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "YAMLNode|Cast")
		int AsInt() { return _convert<int>(); }

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "YAMLNode|Cast")
		float AsFloat() { return _convert<float>(); }

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "YAMLNode|Cast")
		bool AsBool() { return _convert<bool>(); }

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "YAMLNode|Cast")
		FString AsString() { return UTF8_TO_TCHAR(_convert<std::string>().c_str()); }

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "YAMLNode|Cast")
		void AsStringArray(TArray<FString>& strings);


	bool IsValid() const { return _node != nullptr && _node->Type() != YAML::NodeType::Null; }

	/**
	* Test validity of a YAMLNode
	*
	* @param	Test			The object to test
	* @return	Return true if the object is usable
	*/
	UFUNCTION(BlueprintPure, Meta = (CompactNodeTitle = "IsValid"))
		static bool IsValid(const UYAMLNode* Test) { return IsValidT(Test); }

private:
	template <typename T>
	T _convert() {
		try {
			if(!ensure(_node->Type() == YAML::NodeType::Scalar)) return T{};
			return _node->as<T>();
		} catch(YAML::Exception& e) {
			LogMessageOnScreen(e.msg);
			return T{};
		}
	}

	bool HasNode(const std::string& name);

	YAML::Node* _node = nullptr;
};


