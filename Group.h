#pragma once
#include <memory>
#include <vector>
#include "Node.h"

class Group;

using NodePtr = std::shared_ptr<Node>;
using GroupPtr = std::shared_ptr<Group>;

class Group : public Node
{
public:
	void attach(NodePtr nodePtr);
	void detach(NodePtr nodePtr);
	void render(Matrix4 matrix);
	std::vector<NodePtr> children;
};
