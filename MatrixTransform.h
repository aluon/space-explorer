#pragma once
#include "Group.h"

class MatrixTransform : public Group
{
public:
	void render(Matrix4 matrix);
};

