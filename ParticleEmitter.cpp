#include <time.h>
#include "ParticleEmitter.h"
#include <iostream>


ParticleEmitter::ParticleEmitter(int numParticles) : numParticles(numParticles)
{
	particles = std::vector<Particle>(numParticles, Particle());
	for (auto &particle : particles) {
		particle.position = Vector3{ nd(el), 2 * nd(el), nd(el) }.normalize() * 0.2 * spreadScale;
		particle.velocity = Vector3{ nd(el) / 2, 10, nd(el) } * pow(10.0, -2.0) * velocityScale;
		particle.acceleration = Vector3{ 0.0, -2.0, 0.0 } * pow(10.0, -5.0);
		particle.lifespan = lifetime;
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
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glLoadMatrixd((matrix * transform).glMatrix());
	glBegin(GL_QUADS);
	for (auto &particle : particles) {
		if (particle.alive) {
			Vector3 position = particle.acceleration * particle.age * particle.age + particle.velocity * particle.age + particle.position;
			// front/back
			glTexCoord2d(0.0, 0.0);
			glVertex3d(position.x - particleRadius, position.y - particleRadius,  position.z);
			glTexCoord2d(1.0, 0.0);
			glVertex3d(position.x + particleRadius, position.y - particleRadius, position.z);
			glTexCoord2d(1.0, 1.0);
			glVertex3d(position.x + particleRadius, position.y + particleRadius, position.z);
			glTexCoord2d(0.0, 1.0);
			glVertex3d(position.x - particleRadius, position.y + particleRadius, position.z);

			// sides
			glTexCoord2d(0.0, 0.0);
			glVertex3d(position.x, position.y - particleRadius,  position.z + particleRadius);
			glTexCoord2d(1.0, 0.0);
			glVertex3d(position.x, position.y - particleRadius, position.z - particleRadius);
			glTexCoord2d(1.0, 1.0);
			glVertex3d(position.x, position.y + particleRadius, position.z - particleRadius);
			glTexCoord2d(0.0, 1.0);
			glVertex3d(position.x, position.y + particleRadius, position.z + particleRadius);
		}
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
}

void ParticleEmitter::update()
{
	int time = glutGet(GLUT_ELAPSED_TIME);
	double timeElapsed = (time - lastEmitTime) / 1000.0;
	int test = 1 * 0.5;
	int numToEmit = emitRate * timeElapsed;
	if (numToEmit > 0) {
		lastEmitTime = time;
	}

	if (!enabled) {
		numToEmit = 0;
	}

	for (auto &particle : particles) {
		if (!particle.alive) {
			if (numToEmit > 0) {
				particle.age = 0;
				particle.alive = true;
				--numToEmit;
			}
		} else {
			++particle.age;
			if (particle.age > particle.lifespan) {
				particle.alive = false;
			}
		}
	}
}

void ParticleEmitter::reset()
{
	for (auto &particle : particles) {
		particle.alive = false;
		particle.age = 0;
	}
}
