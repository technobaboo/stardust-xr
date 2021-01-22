#include "scenegraph.hpp"
#include "../globals.h"

namespace StardustXRServer {

void Scenegraph::onPathStep(std::string path, std::function<void(std::string)> pathStepFunction) {
	std::istringstream stream(path);
	stream.get(); //Remove beginning slash

	std::string pathStep;
	while(std::getline(stream, pathStep, '/')) {
		pathStepFunction(pathStep);
	}
}

void Scenegraph::sendSignal(int sessionID, std::string path, std::string method, flexbuffers::Reference data) {
	this->executeMethod(sessionID, path, method, data, false);
}
std::vector<uint8_t> Scenegraph::executeMethod(int sessionID, std::string path, std::string method, flexbuffers::Reference args) {
	return this->executeMethod(sessionID, path, method, args, true);
}

void Scenegraph::executeRemoteMethod(uint sessionID, std::string remotePath, std::string remoteMethod, std::vector<uint8_t> args, void *extraData, ServerCallback callback) {
	messengerManager.messengers[sessionID]->executeRemoteMethod(remotePath.c_str(), remoteMethod.c_str(), args, [&](flexbuffers::Reference data) {
		callback(sessionID, data, extraData);
	});
}

void Scenegraph::handleMessengerDeletion(uint sessionID) {
	PropagateFunction messengerDeletionFunction = [&](std::string name, Node *node) {
		if(node->sessionID == sessionID && node->parent) {
			Node *nodeParent = node->parent;
			delete node;
			nodeParent->children.erase(name);
			return false;
		} else {
			node->handleMessengerDeletion(sessionID);
		}
		return true;
	};
	root.propagate("", messengerDeletionFunction);
}

std::vector<uint8_t> Scenegraph::executeMethod(int sessionID, std::string path, std::string method, flexbuffers::Reference args, bool returnValue) {
	//Find the node referenced by the path string
	Node *currentNode = &root;
	this->onPathStep(path, [&](std::string pathStep) {
		currentNode = currentNode->children[pathStep];
		if(currentNode == nullptr) {
			printf("Node %s not found", pathStep.c_str());
			return;
		}
	});

	while(currentNode == nullptr) {}

	if(currentNode->methods[method] == nullptr) {
		printf("Method %s on node %s not found", method.c_str(), path.c_str());
		return std::vector<uint8_t>();
	}
	return (currentNode->methods[method])(sessionID, args, returnValue);
}

void Scenegraph::addNode(std::string path, Node *node) {
	//Get the name of the node to create
	std::string lastNodeName = path.substr(path.find_last_of("/")+1);
	Node *currentNode = &root;

	this->onPathStep(path, [&](std::string pathStep) {

		if(pathStep == lastNodeName)
			currentNode->children[pathStep] = node;
		else if(currentNode->children[pathStep] == nullptr)
			currentNode->children[pathStep] = new Node();

		currentNode->children[pathStep]->parent = currentNode;
		currentNode = currentNode->children[pathStep];

	});
}

Node *Scenegraph::findNode(std::string path) {
	std::string lastNodeName = path.substr(path.find_last_of("/")+1);
	Node *currentNode = &root;
	bool doesNotExist = false;

	this->onPathStep(path, [&](std::string pathStep) {
		if(!doesNotExist)
			currentNode = currentNode->children[pathStep];

		if(!doesNotExist && currentNode == nullptr)
			doesNotExist = true;
	});

	return currentNode;
}

} // namespace StardustXRServer
