#include <time.h>
#include "ParticleSystem.h"
#include <iostream>


ParticleSystem::ParticleSystem(int numParticles) : numParticles(numParticles)
{
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::render(Matrix4 matrix)
{
	std::cout << "timeElapsed: " << timeElapsed << '\n';
	transform *= matrix;
	glBegin(GL_POINTS);
	for (unsigned int i = 0; i < numParticles; ++i) {
		Vector3 vel = Vector3{ urd(el), urd(el), urd(el) };
		Vector3 pos = center + (vel * timeElapsed * 0.01);
		glVertex3d(pos.x, pos.y, pos.z);
	}
	glEnd();
}
