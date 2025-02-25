#pragma once
#include "Collider.h"

class Sphere : public Collider
{
private:
	float _radius = 1.0f;
public:
	Sphere(float radius = 1.0f) : _radius(radius) {}
	~Sphere() = default;
	bool isColliding(const PhysicsObject& object) const override { return false; }
};

