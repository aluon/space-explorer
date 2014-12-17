#include "Camera.h"


void Camera::render(Matrix4 matrix)
{
	updateCamera();
	Group::render(matrix * transform);
}

void Camera::updateCamera()
{
	transform = Transform::lookAt(center, eye, up) * rotMatrix;
}


void Camera::reset(Vector3 center, Vector3 eye, Vector3 up)
{
	this->center = center;
	this->eye = eye;
	this->up = up;
	rotMatrix = Matrix4().identity();
}


Camera::Camera()
{
}
