#include "pch.h"
#include "Sphere.h"

bool Sphere::isColliding(const Collider& other, const DirectX::XMFLOAT3& thisPos, const DirectX::XMFLOAT3& otherPos) const
{
	return other.isCollidingWithSphere(thisPos, otherPos, _radius);
}

bool Sphere::isCollidingWithSphere(const DirectX::XMFLOAT3& spherePos, const DirectX::XMFLOAT3& otherPos, float radius) const
{
    float dx = otherPos.x - spherePos.x;
    float dy = otherPos.y - spherePos.y;
    float dz = otherPos.z - spherePos.z;

    float distance = sqrt(dx * dx + dy * dy + dz * dz);

    return distance <= (_radius + radius);
}

bool Sphere::isCollidingWithPlane(const DirectX::XMFLOAT3& planePos, const DirectX::XMFLOAT3& planeNormal) const
{
    float distance = fabs(planeNormal.x * planePos.x +
        planeNormal.y * planePos.y +
        planeNormal.z * planePos.z) /
        sqrt(planeNormal.x * planeNormal.x +
            planeNormal.y * planeNormal.y +
            planeNormal.z * planeNormal.z);

    return distance <= _radius;
}
