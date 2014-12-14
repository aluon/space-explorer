#include <time.h>
#include "ParticleEmitter.h"
#include <iostream>


ParticleEmitter::ParticleEmitter(int numParticles) : numParticles(numParticles)
{
	particles = std::vector<Particle>(numParticles, Particle());
	for (auto &particle : particles) {
		particle.position = Vector3{ nd(el), nd(el), nd(el) }.normalize();
		particle.velocity = Vector3{ nd(el), 1, 0.0 } * pow(10.0, -3.0);
		particle.acceleration = Vector3{ 0.0, -1.0, 0.0 } * pow(10.0, -6.0);
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
			Vector3 position = particle.acceleration * particle.age * particle.age + particle.velocity * particle.age + particle.position;
			glVertex3d(position.x, position.y, position.z);
		}
	}
	glEnd();
}

void ParticleEmitter::update()
{
	for (auto &particle : particles) {
		++particle.age;
		if (particle.age > particle.lifespan) {
			if (endless) {
				particle.age = 0;
			} else {
				particle.alive = false;
			}
		}
	}
}

void ParticleEmitter::reset()
{
	for (auto &particle : particles) {
		particle.alive = true;
		particle.age = 0;
	}
}
