#pragma once
#include "Collider.h"

class Plane : public Collider
{
public:
	Plane() = default;
	~Plane() = default;
	bool isColliding(const PhysicsObject& object) const override { return false; }
};

