#pragma once
#include <DirectXMath.h>

class Sphere;
class Plane;

class Collider
{
public:
	virtual ~Collider() = default;

	virtual bool isColliding(const Collider& other, const DirectX::XMFLOAT3& thisPos, const DirectX::XMFLOAT3& otherPos) const = 0;

	virtual bool isCollidingWithSphere(const DirectX::XMFLOAT3& spherePos, const DirectX::XMFLOAT3& otherPos, float radius) const = 0;
	virtual bool isCollidingWithPlane(const DirectX::XMFLOAT3& planePos, const DirectX::XMFLOAT3& planeNormal) const = 0;
};
