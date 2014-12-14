#include <time.h>
#include "ParticleEmitter.h"
#include <iostream>


ParticleEmitter::ParticleEmitter(int numParticles) : numParticles(numParticles)
{
	particles = std::vector<Particle>(numParticles, Particle());
	for (auto &particle : particles) {
		particle.position = Vector3{ nd(el), 2 * nd(el), nd(el) }.normalize() * 0.2;
		particle.velocity = Vector3{ nd(el) / 5, 1, nd(el) } * pow(10.0, -4.0);
		particle.acceleration = Vector3{ 0.0, 1.0, 0.0 } * pow(10.0, -7.0);
		particle.lifespan = 15000;
		particle.alive = true;
	}
}


ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::render(Matrix4 matrix)
{
	update();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glLoadMatrixd((matrix * transform).glMatrix());
	glBegin(GL_QUADS);
	glColor3d(0.0, 1.0, 0.0);
	for (auto &particle : particles) {
		if (particle.alive) {
			Vector3 position = particle.acceleration * particle.age * particle.age + particle.velocity * particle.age + particle.position;
			glTexCoord2d(0.0, 0.0);
			glVertex3d(position.x, position.y, position.z);
			glTexCoord2d(1.0, 0.0);
			glVertex3d(position.x + particleRadius, position.y, position.z);
			glTexCoord2d(1.0, 1.0);
			glVertex3d(position.x + particleRadius, position.y + particleRadius, position.z);
			glTexCoord2d(0.0, 1.0);
			glVertex3d(position.x, position.y + particleRadius, position.z);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
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
