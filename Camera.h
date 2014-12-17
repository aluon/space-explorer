#pragma once
#include "MatrixTransform.h"
class Camera : public MatrixTransform
{
public:
	Vector3 center, eye, up;
};

