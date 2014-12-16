#include <GL/glew.h>
#include <Soil/SOIL.h>
#include <iostream>
#include "Skybox.h"

Skybox::Skybox()
{
}

void Skybox::render(Matrix4 matrix)
{
	matrix *= transform;
	glLoadMatrixd(matrix.glMatrix());
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glUseProgram(programId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 12, nullptr);
	glDrawArrays(GL_QUADS, 0, 24);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glUseProgram(0);

	glDisable(GL_CULL_FACE);
}

void Skybox::loadTextures(const std::vector<std::string> &filenames)
{
	float SkyBoxVertices[] =
	{	// x, y, z, x, y, z, x, y, z, x, y, z
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, // +X
		-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, // -X
		-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, // +Y
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, // -Y
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // +Z
		-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f  // -Z
	};

	glGenBuffers(1, &skyboxVbo);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
	glBufferData(GL_ARRAY_BUFFER, 288, SkyBoxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	textures.clear();
	for (const auto &filename : filenames) {
		unsigned int id = SOIL_load_OGL_texture(
			filename.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			0
			);
		if (!id) {
			std::cerr << "Could not load texture " << filename << '\n';
			return;
		}
		textures.push_back(id);
	}

	cubemap = SOIL_load_OGL_cubemap(
		filenames[2].c_str(),
		filenames[0].c_str(),
		filenames[5].c_str(),
		filenames[1].c_str(),
		filenames[4].c_str(),
		filenames[3].c_str(),
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		0
		);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
