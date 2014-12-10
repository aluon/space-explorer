#pragma once
#include <GL\glew.h>
#include <string>
class Shader
{
public:
	Shader();
	~Shader();
};

int loadShader(GLenum shaderType, const std::string &shaderPath);
