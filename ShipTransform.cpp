#include "ShipTransform.h"
#include <algorithm>


ShipTransform::ShipTransform()
{
	reset();
}


ShipTransform::~ShipTransform()
{
}

void ShipTransform::render(Matrix4 matrix)
{
	if (roll > maxRoll) {
		yaw += roll - maxRoll;
		roll = maxRoll;
	} else if (roll < -maxRoll) {
		yaw += roll + maxRoll;
		roll = -maxRoll;
	}
	//pitch = std::min(maxPitch, pitch);
	//transform = Transform::rotateZ(roll) * Transform::rotateY(yaw) * Transform::rotateX(pitch);
	transform = Transform::rotateX(pitch) * Transform::rotateY(yaw) * Transform::rotateZ(roll);
	Group::render(matrix * transform);
}


void ShipTransform::reset()
{
	yaw = roll = pitch = 0.0;
}
