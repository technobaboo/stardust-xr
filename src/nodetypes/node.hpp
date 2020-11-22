#ifndef STARDUSTXR_NODE_H
#define STARDUSTXR_NODE_H

#include <functional>
#include <stardustxr/server/node.hpp>

using namespace StardustXR;
namespace StardustXRServer {

class Node : public ServerNode {
public:
	Node() {}
	virtual ~Node() {}

	virtual void update() = 0;

	void propagate(std::function<void(Node *)> &function);
};

} //namespace StardustXRServer

#endif //STARDUSTXR_NODE_H