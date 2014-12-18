#pragma once
#include <random>
#include "Node.h"
#include "Particle.h"
class ParticleEmitter : public Node
{
public:
	ParticleEmitter(int numParticles);
	~ParticleEmitter();

	virtual void render(Matrix4 matrix) override;

	void update();
	void reset();

	bool enabled = false;

	int emitRate = 1;
	int lastEmitTime = 0;
	double particleRadius = 1;
	GLuint textureId = 0;

	unsigned long int timeElapsed = 0;
	unsigned int numParticles;
	unsigned int lifetime = 200;

	double spreadScale = 1.0;
	double velocityScale = 1.0;

	std::random_device rd;
	std::default_random_engine el{ rd() };
	std::uniform_real_distribution<double> urd{ -1.0, 1.0 };
	std::uniform_real_distribution<double> nd{ -1.0, 1.0 };

	std::vector<Particle> particles;
};

