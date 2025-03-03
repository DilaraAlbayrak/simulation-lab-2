#include "pch.h"
#include "Plane.h"
#include "Sphere.h"

bool Plane::isColliding(const Collider& other) const 
{
    return other.isCollidingWithPlane(*this);
}

bool Plane::isCollidingWithSphere(const Collider& other)  const
{
    const auto* sphere = dynamic_cast<const Sphere*>(&other);
    if (!sphere) return false;

	auto spherePos = sphere->getPosition();
	auto radius = sphere->getRadius();

    float distance = fabs(_normal.x * spherePos.x +
        _normal.y * spherePos.y +
        _normal.z * spherePos.z) /
        sqrt(_normal.x * _normal.x + _normal.y * _normal.y + _normal.z * _normal.z);

    return distance <= radius;
}

bool Plane::isCollidingWithPlane(const Collider& other) const
{
    return false;
}