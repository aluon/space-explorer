#include <fstream>
#include <sstream>
#include "Shader.h"
#include <iostream>


Shader::Shader()
{
}


Shader::~Shader()
{
}

int loadShader(GLenum shaderType, const std::string &shaderPath)
{
	GLuint shader = glCreateShader(shaderType);
	std::ifstream ifs(shaderPath);
	std::stringstream ss;
	ss << ifs.rdbuf();
	std::string shaderString = ss.str();
	const char* shaderSource = shaderString.c_str();
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	GLint shaderGood = false;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderGood);
	if (!shaderGood) {
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar(logLength + 1);
		glGetShaderInfoLog(shader, logLength, NULL, infoLog);
		std::cerr << infoLog << '\n';
		delete infoLog;
	}
	return shader;
}
