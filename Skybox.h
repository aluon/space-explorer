#pragma once
#include "Node.h"
#include "Matrix4.h"
#include <vector>

class Skybox : public Node
{
public:
	Skybox();
	void loadTextures(const std::vector<std::string> &filenames);
	virtual void render(Matrix4 matrix) override;
	std::vector<unsigned int> textures;
	unsigned int cubemap;
};