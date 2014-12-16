#include <GL\glew.h>
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
	glUseProgram(programId);
	glLoadMatrixd(matrix.glMatrix());
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBegin(GL_TRIANGLES);
	{
		for (size_t i = 0; i < vertIndices.size(); ++i) {
			size_t vertIndex = vertIndices[i] * 3;
			size_t normIndex = normIndices[i] * 3;
			size_t texIndex = texIndices[i] * 2;
			glNormal3f(normals[normIndex + 0], normals[normIndex + 1], normals[normIndex + 2]);
			glTexCoord2f(texCoords[texIndex + 0], texCoords[texIndex + 1]);
			glVertex3f(vertices[vertIndex + 0], vertices[vertIndex + 1], vertices[vertIndex + 2]);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glUseProgram(0);
}
