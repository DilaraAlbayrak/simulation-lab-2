#include "pch.h"
#include "Plane.h"

bool Plane::isColliding(const Collider& other, const DirectX::XMFLOAT3& myPos, const DirectX::XMFLOAT3& otherPos) const 
{
    return other.isCollidingWithPlane(myPos, _normal);
}

bool Plane::isCollidingWithSphere(const DirectX::XMFLOAT3& spherePos, float radius) const 
{
    float distance = fabs(_normal.x * spherePos.x +
        _normal.y * spherePos.y +
        _normal.z * spherePos.z) /
        sqrt(_normal.x * _normal.x + _normal.y * _normal.y + _normal.z * _normal.z);

    return distance <= radius;
}

bool Plane::isCollidingWithPlane(const DirectX::XMFLOAT3& planePos, const DirectX::XMFLOAT3& planeNormal) const 
{
    return false;
}