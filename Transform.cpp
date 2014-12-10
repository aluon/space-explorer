#include "Transform.h"

Matrix4 Transform::translate(Vector3 v)
{
	Matrix4 result;
	result.identity();
	result.m[0][3] = v.x;
	result.m[1][3] = v.y;
	result.m[2][3] = v.z;
	return result;
}

// http://www.fastgraph.com/makegames/3drotation/
Matrix4 Transform::rotate(double angle, const Vector3 &axis)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	Matrix4 result = Matrix4().identity();
	double c = cos(angle);
	double s = sin(angle);
	double t = 1 - cos(angle);
	result.m[0][0] = t * axis.x * axis.x + c;
	result.m[0][1] = t * axis.x * axis.y - s * axis.z;
	result.m[0][2] = t * axis.x * axis.z + s * axis.y;
	result.m[1][0] = t * axis.x * axis.y + s * axis.z;
	result.m[1][1] = t * axis.y * axis.y + c;
	result.m[1][2] = t * axis.y * axis.z - s * axis.x;
	result.m[2][0] = t * axis.x * axis.z - s * axis.y;
	result.m[2][1] = t * axis.y * axis.z + s * axis.x;
	result.m[2][2] = t * axis.z * axis.z + c;
	return result;
}

Matrix4 Transform::rotateX(double angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	Matrix4 result;
	result = result.identity();
	result.m[1][1] = cos(angle);
	result.m[2][1] = sin(angle);
	result.m[1][2] = -sin(angle);
	result.m[2][2] = cos(angle);
	return result;
}

Matrix4 Transform::rotateY(double angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	Matrix4 result;
	result = result.identity();
	result.m[0][0] = cos(angle);
	result.m[0][2] = sin(angle);
	result.m[2][0] = -sin(angle);
	result.m[2][2] = cos(angle);
	return result;
}

Matrix4 Transform::rotateZ(double angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	Matrix4 result;
	result = result.identity();
	result.m[0][0] = cos(angle);
	result.m[1][0] = sin(angle);
	result.m[0][1] = -sin(angle);
	result.m[1][1] = cos(angle);
	return result;
}

Matrix4 Transform::scale(double s)
{
	return scale(Vector3(s, s, s));
}

Matrix4 Transform::scale(Vector3 s)
{
	Matrix4 result;
	result.identity();
	result.m[0][0] = s.x;
	result.m[1][1] = s.y;
	result.m[2][2] = s.z;
	return result;
}

Matrix4 Transform::lookAt(Vector3 center, Vector3 eye, Vector3 up)
{
	Matrix4 result;
	Vector3 z = (center - eye).normalize();
	Vector3 x = cross(up, z).normalize();
	Vector3 y = cross(z, x);
	result.m[0][0] = x.x;
	result.m[0][1] = x.y;
	result.m[0][2] = x.z;
	result.m[1][0] = y.x;
	result.m[1][1] = y.y;
	result.m[1][2] = y.z;
	result.m[2][0] = z.x;
	result.m[2][1] = z.y;
	result.m[2][2] = z.z;
	result.m[3][3] = 1;
	result = result.transpose();
	result *= Transform::translate(center.negate());
	return result;
}

#include <iostream>
Matrix4 Transform::perspective(double fov, double aspect, double near, double far)
{
	Matrix4 result;
	fov = fov / 180.0 * M_PI;  // convert from degrees to radians
	result.m[1][1] = 1 / tan(0.5 * fov);
	result.m[0][0] = result.m[1][1] / aspect;
	result.m[2][2] = (near + far) / (near - far);
	result.m[2][3] = (2 * near * far) / (near - far);
	result.m[3][2] = -1;
	return result;
}

Matrix4 Transform::viewport(int x, int y, int width, int height)
{
	Matrix4 result;
	result.m[0][0] = (width - x) / 2;
	result.m[0][3] = (width + x) / 2;
	result.m[1][1] = (height - y) / 2;
	result.m[1][3] = (height + y) / 2;
	result.m[2][2] = 1 / 2;
	result.m[2][3] = 1 / 2;
	result.m[3][3] = 1;
	return result;
}
