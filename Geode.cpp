#include "Geode.h"
#include "Window.h"

const double unit = 1.0;

Geode::Geode()
{
}

void Sphere::render(Matrix4 matrix)
{
	glLoadMatrixd((matrix * transform).glMatrix());
	glutSolidSphere(unit, 60.0, 40.0);
}

void Cube::render(Matrix4 matrix)
{
	glLoadMatrixd((matrix * transform).glMatrix());
	glutSolidCube(2 * unit);
}

void Cone::render(Matrix4 matrix)
{
	glLoadMatrixd((matrix * transform).glMatrix());
	glutSolidCone(unit / 2.0, unit, 30, 20);;
}
