#include <iostream>
#include "Vector3.h"

Vector3::Vector3() : x(0), y(0), z(0)
{
	return;
}

Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z)
{
	return;
}

Vector3::Vector3(const Vector4 &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

Vector3 Vector3::operator+(const Vector3 &rh)
{
	return Vector3(x + rh.x, y + rh.y, z + rh.z);
}

Vector3 Vector3::operator-(const Vector3 &rh)
{
	return Vector3(x - rh.x, y - rh.y, z - rh.z);
}

Vector3 Vector3::negate()
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::scale(double s) const
{
	return Vector3(s * x, s * y, s * z);
}

double Vector3::length() const
{
	return std::sqrt(x*x + y*y + z*z);
}

Vector3 Vector3::normalize() const
{
	return scale(1.0 / length());
}

void Vector3::print(std::string comment) const
{
	std::cout << comment << " <" << x << ", " << y << ", " << z << ">" << std::endl;
}

Vector3& Vector3::operator=(const Vector3 &rh)
{
	x = rh.x;
	y = rh.y;
	z = rh.z;
	return (*this);
}

Vector3& Vector3::operator-=(const Vector3 &rh)
{
	(*this) = (*this) - rh;
	return (*this);
}

Vector3& Vector3::operator+=(const Vector3 &rh)
{
	(*this) = (*this) + rh;
	return (*this);
}

Vector3 Vector3::operator*(const double &rh)
{
	return scale(rh);
}

Vector3 Vector3::operator-()
{
	return negate();
}

Vector3 cross(const Vector3 &lh, const Vector3 &rh)
{
	return Vector3(
		lh.y * rh.z - lh.z * rh.y,
		lh.z * rh.x - lh.x * rh.z,
		lh.x * rh.y - lh.y * rh.x);
}

double dot(const Vector3 &lh, const Vector3 &rh)
{
	return lh.x * rh.x + lh.y * rh.y + lh.x * rh.y;
}