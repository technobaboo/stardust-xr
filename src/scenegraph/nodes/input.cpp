#include "input.hpp"
#include "../../globals.h"

namespace StardustXRServer {

InputInterface::InputInterface() {
	children["handler"] = new Node();
	children["handler"]->parent = this;

	children["method"] = new Node();
	children["method"]->parent = this;

	STARDUSTXR_NODE_METHOD("registerInputHandler", &InputInterface::registerInputHandler)
}
InputInterface::~InputInterface() {}

std::vector<uint8_t> InputInterface::registerInputHandler(uint sessionID, flexbuffers::Reference data, bool) {
	flexbuffers::Vector vec    = data.AsVector();
	std::string name           = vec[0].AsString().str();
	std::string field          = vec[1].AsString().str();
	std::string callbackPath   = vec[2].AsString().str();
	std::string callbackMethod = vec[3].AsString().str();

	InputHandler *handler               = new InputHandler();
	handler->ready                      = false;
	handler->parent                     = children["handler"];
	children["handler"]->children[name] = handler;
	handler->sessionID                  = sessionID;
	handler->field                      = dynamic_cast<Field *>(scenegraph.findNode(field));
	handler->callbackPath               = callbackPath;
	handler->callbackMethod             = callbackMethod;
	handler->ready                      = true;

	inputHandlers.pushBack(handler);
	inputHandlers.done();

	return std::vector<uint8_t>();
}

void InputInterface::processInput() {
	const uint32_t inputMethodCount = inputMethods.length();
	const uint32_t inputHandlerCount = inputHandlers.length();

	for(uint32_t i=0; i<inputMethodCount; ++i) {
		std::list<DistanceLink> distanceLinks;
		for(uint32_t j=0; j<inputHandlerCount; ++j) {
			distanceLinks.push_front({
				inputMethods[i],
				inputMethods[i]->distanceTo(inputHandlers[j]),
				inputHandlers[j]
			});
		}
		distanceLinks.sort();
		std::vector<uint8_t> inputData = distanceLinks.begin()->method->serialize(distanceLinks.begin()->distance);
		distanceLinks.begin()->handler->sendInput(
			distanceLinks,
			inputData
		);
	}

	inputMethods.done();
	inputHandlers.done();
}

} // namespace StardustXRServer
