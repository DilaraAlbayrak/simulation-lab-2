#pragma once

class PhysicsObject;

class Collider
{
public:
	virtual ~Collider() = default;

	virtual bool isColliding(const PhysicsObject& object) const = 0;
};

