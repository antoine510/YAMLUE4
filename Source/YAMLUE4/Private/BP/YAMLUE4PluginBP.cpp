#include "BP/YAMLUE4PluginBP.h"
#include <Misc/FileHelper.h>
#include <Misc/Paths.h>

UYAMLNode::UYAMLNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UYAMLNode::LoadDocument(const FString& path, UYAMLNode*& root) {
	root = NewObject<UYAMLNode>();
	FString fileContent;
	FFileHelper::LoadFileToString(fileContent, *FPaths::Combine(FPaths::ProjectContentDir(), path));
	try {
		root->_node = new YAML::Node(YAML::Load(TCHAR_TO_UTF8(*fileContent)));
	} catch(YAML::ParserException & e) {
		LogMessageOnScreen(e.msg);
		delete root;
		root = nullptr;
	}
}

void UYAMLNode::GetNode(const FString& name, UYAMLNode*& node) {
	try {
		if(!ensure(_node->Type() == YAML::NodeType::Map)) return;
		std::string stdname = std::string(TCHAR_TO_UTF8(*name));
		if(!ensure(HasNode(stdname))) return;
		node = NewObject<UYAMLNode>();
		node->_node = new YAML::Node((*_node)[stdname]);
	} catch(YAML::Exception & e) {
		LogMessageOnScreen(e.msg);
		delete node;
		node = nullptr;
	}
}

bool UYAMLNode::GetNodeFallback(const FString& name, const FString& fallback, UYAMLNode*& node) {
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
	} catch(YAML::Exception & e) {
		LogMessageOnScreen(e.msg);
		delete node;
		node = nullptr;
		return false;
	}
}

void UYAMLNode::GetNodeArray(const FString& name, TArray<UYAMLNode*>& nodeArray) {
	try {
		if(!ensure(_node->Type() == YAML::NodeType::Map)) return;
		std::string stdname = std::string(TCHAR_TO_UTF8(*name));
		if(!ensure(HasNode(stdname))) return;
		if((*_node)[stdname].Type() == YAML::NodeType::Null) return;
		for(const YAML::Node& n : (*_node)[stdname]) {
			auto idx = nodeArray.Add(NewObject<UYAMLNode>());
			nodeArray[idx]->_node = new YAML::Node(n);
		}
	} catch(YAML::Exception & e) {
		LogMessageOnScreen(e.msg);
		for(UYAMLNode* n : nodeArray) delete n;
		nodeArray.Empty();
	}
}

void UYAMLNode::AsStringArray(TArray<FString>& strings) {
	try {
		if(!ensure(_node->Type() == YAML::NodeType::Sequence)) return;
		for(const YAML::Node& n : *_node) {
			strings.Add(UTF8_TO_TCHAR(n.Scalar().c_str()));
		}
	} catch(YAML::Exception& e) {
		LogMessageOnScreen(e.msg);
	}
}

bool UYAMLNode::HasNode(const std::string& name) {
	try {
		if(!ensure(_node->Type() == YAML::NodeType::Map)) return false;
		for(auto sub : *_node) {
			if(sub.first.Type() == YAML::NodeType::Scalar && sub.first.Scalar() == name) return true;
		}
		return false;
	} catch(YAML::Exception & e) {
		LogMessageOnScreen(e.msg);
		return false;
	}
}
