#pragma once
#include "Node.h"
class Geode : public Node
{
public:
	Geode();
};

class Sphere : public Geode
{
	void render(Matrix4 matrix);
};

class Cube : public Geode
{
	void render(Matrix4 matrix);
};

class Cone : public Geode
{
	void render(Matrix4 matrix);
};
