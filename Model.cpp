#include "Model.h"

Model::Model()
{
	transform = transform.identity();
}

Vector3 Model::getCenter()
{
	return (min + max).scale(0.5);
}

Vector3 Model::getDimensions()
{
	return Vector3(abs(max.x - min.x), abs(max.y - min.y), abs(max.z - min.z));
}

void Model::render(Matrix4 matrix)
{
	matrix *= transform;
	glLoadMatrixd(matrix.glMatrix());
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices.data());
	if (!normals.empty()) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, normals.data());
	}
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, colors.data());
	glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, faces.data());
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
