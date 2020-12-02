#include "lifecycle.hpp"
#include "../scenegraph.hpp"
#include "../../globals.h"
#include <stereokit.h>

namespace StardustXRServer {

LifeCycleInterface::LifeCycleInterface() {
	STARDUSTXR_NODE_METHOD("subscribeLogicStep", &LifeCycleInterface::subscribeLogicStep)

	prevFrameTime = sk::time_get();
	frameTime = sk::time_get();
}

void LifeCycleInterface::sendLogicStepSignals() {
	frameTime = sk::time_get();
	double delta = frameTime - prevFrameTime;

	lifeCycleUpdateMethodList.lock();
	lifeCycleUpdateMethodList.forEach([delta](uint32_t index, LifeCycleUpdateMethod &method) {
		messengerManager.messengers[method.sessionID]->sendSignal(
			method.nodePath.c_str(),
			method.methodName.c_str(),
			[&](flexbuffers::Builder &fbb) {
				fbb.Vector([&]() {
					fbb.Double(delta);
					fbb.Double(0);
				});
			}
		);
	});
	lifeCycleUpdateMethodList.unlock();

	prevFrameTime = frameTime;
}

void LifeCycleInterface::handleMessengerDeletion(uint sessionID) {
	lifeCycleUpdateMethodList.lock();
	lifeCycleUpdateMethodList.forEach([&](uint32_t index, LifeCycleUpdateMethod &method) {
		if(method.sessionID == sessionID) {
			lifeCycleUpdateMethodList.erase(index);
		}
	});
	lifeCycleUpdateMethodList.unlock();
}

std::vector<uint8_t> LifeCycleInterface::subscribeLogicStep(uint sessionID, flexbuffers::Reference data, bool returnValue) {
	flexbuffers::Vector vector = data.AsVector();
	LifeCycleUpdateMethod logicStepMethod = {
		sessionID,
		vector[0].AsString().str(),
		vector[1].AsString().str()
	};

	lifeCycleUpdateMethodList.lock();
	lifeCycleUpdateMethodList.pushBack(logicStepMethod);
	lifeCycleUpdateMethodList.unlock();

	return std::vector<uint8_t>();
}

} // namespace StardustXRServer
