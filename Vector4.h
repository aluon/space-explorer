#pragma once
#include <string>

class Matrix4;

class Vector4
{
public:
	Vector4();
	Vector4(double x, double y, double z, double w);
	Vector4 operator+(const Vector4 &);
	Vector4 operator-(const Vector4 &);
	Vector4 operator*(const Matrix4 &);
	double operator*(const Vector4 &);
	double length();
	void dehomogenize();
	void print(std::string prefix) const;
	double x, y, z, w;
};