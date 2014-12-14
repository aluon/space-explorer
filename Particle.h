#pragma once
#include "Vector3.h"
class Particle
{
public:
	Particle();
	~Particle();

	Vector3 position, velocity, acceleration;
	bool alive = false;
	unsigned int age = 0, lifespan = 0;
};

