#pragma once
#include <string>
#include "Vector4.h"

class Vector3
{
public:
	Vector3();
	Vector3(const Vector4 &v);
	Vector3(double, double, double);
	Vector3 operator+(const Vector3 &);
	Vector3 operator-(const Vector3 &);
	Vector3& operator=(const Vector3 &);
	Vector3& operator-=(const Vector3 &);
	Vector3& operator+=(const Vector3 &);
	Vector3 operator*(const double &rh);
	Vector3 operator-();
	Vector3 negate();
	Vector3 scale(double) const;
	double length() const;
	Vector3 normalize() const;
	void print(std::string comment) const;
	double x, y, z;
};

double dot(const Vector3 &, const Vector3 &);
Vector3 cross(const Vector3 &, const Vector3 &);