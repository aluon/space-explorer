#pragma once
#include "MatrixTransform.h"
class ShipTransform : public MatrixTransform
{
public:
	ShipTransform();
	~ShipTransform();

	virtual void render(Matrix4 matrix) override;

	double pitch, roll, yaw;
	double maxPitch = 30.0, maxRoll = 30.0;
	void reset();
};

