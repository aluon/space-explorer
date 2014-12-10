#include "Light.h"

Light::Light(GLenum name) : name(name)
{
}

Light::~Light()
{
	glDisable(name);
}

void Light::render(Matrix4 matrix)
{
	glLoadMatrixd((matrix * transform).glMatrix());
	/*
	GLfloat lightPos[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(name, GL_POSITION, lightPos);
	*/
	GLfloat ambientColor[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightPos[] = { 1.0, 1.0, 1.0, 0.0 };
	glLightfv(name, GL_POSITION, lightPos);
	glLightfv(name, GL_AMBIENT, ambientColor);
	glEnable(name);
	glColor3d(0.0, 1.0, 0.0);
	//glutWireSphere(0.1, 30, 20);
}

