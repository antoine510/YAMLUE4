#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

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
		static void LoadDocument(const FString& path, UYAMLNode*& root) {
		root = NewObject<UYAMLNode>();
		FString fileContent;
		FFileHelper::LoadFileToString(fileContent, *FPaths::Combine(FPaths::ProjectContentDir(), path));
		try {
			root->_node = new YAML::Node(YAML::Load(TCHAR_TO_UTF8(*fileContent)));
		} catch(YAML::ParserException& e) {
			LogMessageOnScreen(e.msg);
			delete root;
			root = nullptr;
		}
	}

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "YAMLNode|Cast")
		void GetNode(const FString& name, UYAMLNode*& node) {
		try {
			if(!ensure(_node->Type() == YAML::NodeType::Map)) return;
			std::string stdname = std::string(TCHAR_TO_UTF8(*name));
			if(!ensure(HasNode(stdname))) return;
			node = NewObject<UYAMLNode>();
			node->_node = new YAML::Node((*_node)[stdname]);
		} catch(YAML::Exception& e) {
			LogMessageOnScreen(e.msg);
			delete node;
			node = nullptr;
		}
	}

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "YAMLNode|Cast")
		bool GetNodeFallback(const FString& name, const FString& fallback, UYAMLNode*& node) {
		try {
			if(!ensure(_node->Type() == YAML::NodeType::Map)) return false;
			node = NewObject<UYAMLNode>();
			std::string stdname = std::string(TCHAR_TO_UTF8(*name));
			if(!HasNode(stdname)) {
				node->_node = new YAML::Node(YAML::Load(std::string(TCHAR_TO_UTF8(*fallback))));
				return false;
			} else {
				node->_node = new YAML::Node((*_node)[stdname]);
				return true;
			}
		} catch(YAML::Exception& e) {
			LogMessageOnScreen(e.msg);
			delete node;
			node = nullptr;
			return false;
		}
	}

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "Has", Keywords = "check has", BlueprintAutocast), Category = "YAMLNode|Check")
		bool HasNode(const FString& name) {
		return HasNode(std::string(TCHAR_TO_UTF8(*name)));
	}

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "YAMLNode|Cast")
		void GetNodeArray(const FString& name, TArray<UYAMLNode*>& nodeArray) {
		try {
			if(!ensure(_node->Type() == YAML::NodeType::Map)) return;
			std::string stdname = std::string(TCHAR_TO_UTF8(*name));
			if(!ensure(HasNode(stdname))) return;
			if((*_node)[stdname].Type() == YAML::NodeType::Null) return;
			for(const YAML::Node& n : (*_node)[stdname]) {
				auto idx = nodeArray.Add(NewObject<UYAMLNode>());
				nodeArray[idx]->_node = new YAML::Node(n);
			}
		} catch(YAML::Exception& e) {
			LogMessageOnScreen(e.msg);
			for(UYAMLNode* n : nodeArray) delete n;
			nodeArray.Empty();
		}
	}

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "YAMLNode|Cast")
		int AsInt() { return _convert<int>(); }

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "YAMLNode|Cast")
		float AsFloat() { return _convert<float>(); }

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "YAMLNode|Cast")
		bool AsBool() { return _convert<bool>(); }

	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "YAMLNode|Cast")
		FString AsString() { return UTF8_TO_TCHAR(_convert<std::string>().c_str()); }


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

	bool HasNode(const std::string& name) {
		try {
			if(!ensure(_node->Type() == YAML::NodeType::Map)) return false;
			for(auto sub : *_node) {
				if(sub.first.Type() == YAML::NodeType::Scalar && sub.first.Scalar() == name) return true;
			}
			return false;
		} catch(YAML::Exception& e) {
			LogMessageOnScreen(e.msg);
			return false;
		}
	}

	YAML::Node* _node = nullptr;
};


