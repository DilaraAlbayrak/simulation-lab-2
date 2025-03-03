#include "pch.h"
#include "Sphere.h"
#include "Plane.h"

bool Sphere::isColliding(const Collider& other) const
{
	return other.isCollidingWithSphere(*this);
}

bool Sphere::isCollidingWithSphere(const Collider& other) const
{
    const auto* sphere = dynamic_cast<const Sphere*>(&other);
	if (!sphere) return false;

	float dx = other.getPosition().x - getPosition().x;
	float dy = other.getPosition().y - getPosition().y;
	float dz = other.getPosition().z - getPosition().z;

	float distanceSquared = pow(dx, 2) + pow(dy, 2) + pow(dz, 2);
	float sumRadiiSquared = pow(_radius + sphere->getRadius(), 2);

    return distanceSquared <= sumRadiiSquared;
}

bool Sphere::isCollidingWithPlane(const Collider& other) const
{
    const Plane* plane = dynamic_cast<const Plane*>(&other);
    if (!plane) return false;

    auto planeNormal = plane->getNormal();
    auto planePos = plane->getPosition();

    float distance = fabs(planeNormal.x * planePos.x +
        planeNormal.y * planePos.y +
        planeNormal.z * planePos.z) /
        sqrt(planeNormal.x * planeNormal.x +
            planeNormal.y * planeNormal.y +
            planeNormal.z * planeNormal.z);

    return distance <= _radius;
}
