#pragma once
#include "MatrixTransform.h"
class Camera : public MatrixTransform
{
public:
	Vector3 rotAxis{ 1.0, 0.0, 0.0 };
	double rotAngle = 0.0;
	Matrix4 rotMatrix = Matrix4().identity();
	Vector3 center, eye, up;

	virtual void render(Matrix4 matrix) override;
	void updateCamera();
	void reset(Vector3 center, Vector3 eye, Vector3 up);
	Camera();
};

