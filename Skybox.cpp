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
	static const GLint faces[6][4] = {
		{ 5, 1, 2, 6 },
		{ 5, 4, 0, 1 },
		{ 0, 4, 7, 3 },
		{ 4, 5, 6, 7 },
		{ 1, 0, 3, 2 },
		{ 2, 3, 7, 6 } 
	};

	double v[8][3];
	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1; 
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1; 
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -1;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = 1; 

	for (int i = 5; i >= 0; i--)
	{
		if (i == 1) continue;	// don't texture bottom face
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 1);  glVertex3dv(&v[faces[i][0]][0]);
			glTexCoord2f(1, 1);  glVertex3dv(&v[faces[i][1]][0]);
			glTexCoord2f(1, 0);  glVertex3dv(&v[faces[i][2]][0]);
			glTexCoord2f(0, 0);  glVertex3dv(&v[faces[i][3]][0]);
		}
		glEnd();
	}
}

void Skybox::loadTextures(const std::vector<std::string> &filenames)
{
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
