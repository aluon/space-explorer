#include "Group.h"

void Group::attach(NodePtr nodePtr)
{
	children.push_back(std::static_pointer_cast<Node>(nodePtr));
}

void Group::render(Matrix4 matrix)
{
	for (auto child : children) {
		glLoadMatrixd(matrix.glMatrix());
		child->render(matrix);
	}
}

void Group::detach(NodePtr nodePtr)
{
	auto child = std::find(children.begin(), children.end(), nodePtr);
	children.erase(child);
}
