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

	Vector3 center;
	double timeBias;
	unsigned long int timeElapsed = 0;
	unsigned int numParticles;
	std::random_device rd;
	std::default_random_engine el{ rd() };
	std::uniform_real_distribution<double> urd{ -1.0, 1.0 };

	std::vector<Particle> particles;
	void update();
};

