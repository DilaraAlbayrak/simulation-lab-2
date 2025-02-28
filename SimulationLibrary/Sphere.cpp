#include "pch.h"
#include "Sphere.h"

bool Sphere::isColliding(const Collider& other, const DirectX::XMFLOAT3& thisPos, const DirectX::XMFLOAT3& otherPos) const
{
	return other.isCollidingWithSphere(thisPos, _radius);
}

bool Sphere::isCollidingWithSphere(const DirectX::XMFLOAT3& spherePos, float radius) const
{
    float distance = sqrt(pow(spherePos.x - spherePos.x, 2) +
        pow(spherePos.y - spherePos.y, 2) +
        pow(spherePos.z - spherePos.z, 2));

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
