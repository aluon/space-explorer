#include <time.h>
#include "ParticleEmitter.h"
#include <iostream>


ParticleEmitter::ParticleEmitter(int numParticles) : numParticles(numParticles)
{
	particles = std::vector<Particle>(numParticles, Particle());
	for (auto &particle : particles) {
		particle.position = { urd(el), 0.0, 0.0 };
		particle.velocity = Vector3{ urd(el), 1, 0.0 } * pow(10.0, -6.0);
		particle.acceleration = Vector3{ 0.0, -1.0, 0.0 } * pow(10.0, -9.0);
		particle.lifespan = 10000;
		particle.alive = true;
	}
}


ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::render(Matrix4 matrix)
{
	update();
	glLoadMatrixd((matrix * transform).glMatrix());
	glBegin(GL_POINTS);
	glColor3d(0.0, 1.0, 0.0);
	for (auto &particle : particles) {
		if (particle.alive) {
			glVertex3d(particle.position.x, particle.position.y, particle.position.z);
		}
	}
	glEnd();
}

void ParticleEmitter::update()
{
	for (auto &particle : particles) {
		particle.position += particle.acceleration * particle.age * particle.age + particle.velocity * particle.age;

		++particle.age;
		if (particle.age > particle.lifespan) {
			particle.alive = false;
		}
	}
}
