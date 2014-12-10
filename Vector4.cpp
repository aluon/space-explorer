#include <iostream>
#include "Vector4.h"
#include "Matrix4.h"

Vector4::Vector4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w)
{
	return;
}

Vector4::Vector4() : x(0), y(0), z(0), w(0)
{
	return;
}

Vector4 Vector4::operator+(const Vector4 &rh)
{
	Vector4 result(
		x + rh.x,
		y + rh.y,
		z + rh.z,
		w + rh.w);
	return result;
}

Vector4 Vector4::operator-(const Vector4 &rh)
{
	Vector4 result(
		x - rh.x,
		y - rh.y,
		z - rh.z,
		w - rh.w);
	return result;
}

void Vector4::dehomogenize()
{
	if (w == 0) return;
	x /= w;
	y /= w;
	z /= w;
	w = 1;
}

void Vector4::print(std::string prefix) const
{
	std::cout << prefix << " <" << x << ", " << y << ", " << z << ", " << w << ">" << std::endl;
}

double Vector4::length()
{
	return sqrt(x*x + y*y + z*z);
}

Vector4 Vector4::operator*(const Matrix4 &m)
{
	return Vector4(
		m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3] * w,
		m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z + m.m[1][3] * w,
		m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z + m.m[2][3] * w,
		m.m[3][0] * x + m.m[3][1] * y + m.m[3][2] * z + m.m[3][3] * w);
}

double Vector4::operator*(const Vector4 &v)
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}
