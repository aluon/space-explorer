#pragma once
#include <vector>
#include "Node.h"

class Light : public Node
{
public:
	Light(GLenum lightName);
	~Light();
	void render(Matrix4 matrix);
	GLenum name = GL_LIGHT0;
};

