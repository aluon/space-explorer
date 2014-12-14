#include <math.h>
#include <iostream>
#include <iomanip>

#include "Matrix4.h"

Matrix4::Matrix4()
{
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      m[i][j] = 0;
    }
  }
} 

Matrix4::Matrix4(std::initializer_list<double> init)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			size_t index = i * 4 + j;
			if (index < init.size()) {
				m[i][j] = *(init.begin() + index);
			} else {
				m[i][j] = 0;
			}
		}
	}
}

Matrix4& Matrix4::operator=(const Matrix4& m2)
{
  if (this != &m2)
  {
    for (int i=0; i<4; ++i)
    {
      for (int j=0; j<4; ++j)
      {
        m[i][j] = m2.m[i][j];
      }
    }
  }
  return *this;
}

// return pointer to matrix elements
double* Matrix4::getPointer()
{
  return &m[0][0];
}

// set matrix to identity matrix
Matrix4 Matrix4::identity()
{
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      if (i==j) 
        m[i][j] = 1.0; 
      else 
        m[i][j] = 0.0;
    }
  }
  return *this;
}

// transpose the matrix (mirror at diagonal)
Matrix4 Matrix4::transpose()
{
  Matrix4 result;
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      result.m[j][i] = m[i][j];
    }
  }
  return result;
}

Matrix4 Matrix4::operator*(const Matrix4 &rh)
{
	Matrix4 result;
	const double *lhPtr, *rhPtr;
	double *rsPtr = result.getPointer();						// result matrix
	lhPtr = const_cast<const double *>(this->getPointer());		// beginning of lh matrix
	rhPtr = reinterpret_cast<const double *>(rh.m);				// beginning of rh matrix
	
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			*rsPtr = lhPtr[0] * rhPtr[j + 0 * 4]
				+ lhPtr[1] * rhPtr[j + 1 * 4]
				+ lhPtr[2] * rhPtr[j + 2 * 4]
				+ lhPtr[3] * rhPtr[j + 3 * 4];
			++rsPtr;
		}
		lhPtr += 4;
	}
	return result;
}

Vector3 Matrix4::operator*(const Vector3 &v)
{
	Vector4 rh(v.x, v.y, v.z, 1);
	Vector4 result = (*this) * rh;
	return Vector3(result.x, result.y, result.z);
}

Vector4 Matrix4::operator*(const Vector4 &v)
{
	return Vector4(
		m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
		m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
		m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
		m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w);
}

void Matrix4::print(std::string comment)
{
	std::cout << comment << " [" << std::endl;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << std::setprecision(4) << std::setw(8) << m[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "]" << std::endl;
}

Matrix4& Matrix4::operator+=(const Matrix4 &rh)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m[i][j] += rh.m[i][j];
		}
	}
	return *this;
}

Matrix4 Matrix4::operator+(const Matrix4 &rh)
{
	Matrix4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] += rh.m[i][j];
		}
	}
	return result;
}

Matrix4& Matrix4::operator*=(const Matrix4 &rh)
{
	*this = (*this) * rh;
	return *this;
}

double* Matrix4::glMatrix()
{
	return transpose().getPointer();
}


Matrix4 Matrix4::billboardCylindrical()
{
	Matrix4 result = *this;
	result.m[0][0] = 1.0;
	result.m[0][1] = 0.0;
	result.m[0][2] = 0.0;
	result.m[2][0] = 0.0;
	result.m[2][1] = 0.0;
	result.m[2][2] = 1.0;
	return result;
}


Matrix4 Matrix4::billboardSpherical()
{
	Matrix4 result = *this;
	result.m[0][0] = 1.0;
	result.m[0][1] = 0.0;
	result.m[0][2] = 0.0;
	result.m[1][0] = 0.0;
	result.m[1][1] = 1.0;
	result.m[1][2] = 0.0;
	result.m[2][0] = 0.0;
	result.m[2][1] = 0.0;
	result.m[2][2] = 1.0;
	return Matrix4();
}
