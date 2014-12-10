#pragma once

#include <vector>
#include "Matrix4.h"
#include "Geode.h"
#include <memory>

struct Face
{
	int v1, v2, v3;
};

class Model : public Geode
{
public:
	Model();
	Vector3 getCenter();
	Vector3 getDimensions();
	void render(Matrix4 matrix);

	Matrix4 transform;
	std::vector<float> vertices, normals, colors, texCoords;
	std::vector<unsigned int> vertIndices, normIndices, texIndices;
	Vector3 min, max;
};

using ModelPtr = std::shared_ptr<Model>;
