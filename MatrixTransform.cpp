#include "MatrixTransform.h"

void MatrixTransform::render(Matrix4 matrix)
{
	Group::render(matrix * transform);
}
