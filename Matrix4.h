#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#define M_PI 3.14159265358979323846
#include <string>

#include "Vector3.h"

class Vector4;

class Matrix4
{
public:
	Matrix4();
	Matrix4(std::initializer_list<double> init);
	Matrix4(Vector3 pos);
	Matrix4& operator=(const Matrix4&);
	Matrix4 Matrix4::operator+(const Matrix4 &rh);
	Matrix4& operator+=(const Matrix4 &);
	Matrix4 operator*(const Matrix4 &);
	Matrix4& operator*=(const Matrix4 &);
	Vector3 operator*(const Vector3 &v);
	Vector4 operator*(const Vector4 &v);
	double* getPointer();
	double* glMatrix();
	Matrix4 identity();
	Matrix4 transpose();
	void print(std::string);
	double m[4][4];   // matrix elements; first index is for rows, second for columns (row-major)
};

#endif