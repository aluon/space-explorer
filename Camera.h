#pragma once
#include "MatrixTransform.h"
class Camera : public MatrixTransform
{
public:
	Vector3 rotAxis{ 1.0, 0.0, 0.0 };
	double rotAngle = 15.0;
	Vector3 center, eye, up;

	virtual void render(Matrix4 matrix) override;

};

