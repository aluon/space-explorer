#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Parser.h"

Model parseModel(const std::string &fileName)
{
	std::cout << "Loading " << fileName << '\n';

	Model model;
	std::ifstream ifs(fileName);
	char line[255];

	float xMax = std::numeric_limits<float>::min();
	float yMax = std::numeric_limits<float>::min();
	float zMax = std::numeric_limits<float>::min();
	float xMin = std::numeric_limits<float>::max();
	float yMin = std::numeric_limits<float>::max();
	float zMin = std::numeric_limits<float>::max();

	while (ifs.getline(line, sizeof(line), '\n')) {
		if (strncmp(line, "#", 1) == 0) {
			continue;
		} else if (strncmp(line, "", 1) == 0) {
			continue;
		} else if (strncmp(line, "v ", 2) == 0) {
			float x, y, z, r, g, b;
			if (sscanf_s(line, "v %f %f %f %f %f %f", &x, &y, &z, &r, &g, &b) == 6) {
				model.colors.push_back(r);
				model.colors.push_back(g);
				model.colors.push_back(b);
			}
			model.vertices.push_back(x);
			model.vertices.push_back(y);
			model.vertices.push_back(z);
			xMax = std::max(xMax, x);
			yMax = std::max(yMax, y);
			zMax = std::max(zMax, z);
			xMin = std::min(xMin, x);
			yMin = std::min(yMin, y);
			zMin = std::min(zMin, z);
		} else if (strncmp(line, "vn ", 3) == 0) {
			float x, y, z;
			sscanf_s(line, "vn %f %f %f", &x, &y, &z);
			model.normals.push_back(x);
			model.normals.push_back(y);
			model.normals.push_back(z);
		} else if (strncmp(line, "f ", 2) == 0) {
			int x, y, z;
			if (sscanf_s(line, "f %d//%*d %d//%*d %d//%*d", &x, &y, &z) == 9) {
			} else if (sscanf_s(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &x, &y, &z) == 6) {
			} else {
				sscanf_s(line, "f %d %d %d", &x, &y, &z);
			}
			model.faces.push_back(x - 1);
			model.faces.push_back(y - 1);
			model.faces.push_back(z - 1);
		}
	}
	model.max = Vector3(xMax, yMax, zMax);
	model.min = Vector3(xMin, yMin, zMin);
	return model;
}
