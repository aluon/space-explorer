#pragma once
#include <stdlib.h>
#include <GL/glut.h>
#include "Transform.h"

class Node
{
public:
	Node();
	virtual void render(Matrix4 matrix) = 0;
	Matrix4 transform;
	Node *parent = nullptr;
};
