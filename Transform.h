#pragma once
#include "Matrix4.h"
#include "Vector3.h"

namespace Transform
{
	Matrix4 rotateX(double angle);
	Matrix4 rotateY(double angle);
	Matrix4 rotateZ(double angle);
	Matrix4 translate(Vector3 v);
	Matrix4 scale(double s);
	Matrix4 scale(Vector3 s);

	Matrix4 lookAt(Vector3 center, Vector3 eye, Vector3 up);
	Matrix4 perspective(double fov, double aspect, double near, double far);
	Matrix4 viewport(int x, int y, int width, int height);
	Matrix4 rotate(double angle, const Vector3 &axis);
};

