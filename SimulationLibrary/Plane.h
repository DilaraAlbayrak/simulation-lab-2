#pragma once
#include "Collider.h"

class Plane : public Collider
{
private:
	DirectX::XMFLOAT3& _normal;
public:
	Plane(DirectX::XMFLOAT3 normal = { 0.0f, 1.0f, 0.0f }) : _normal(normal) {}
	~Plane() = default;

	bool isColliding(const Collider& other, const DirectX::XMFLOAT3& thisPos, const DirectX::XMFLOAT3& otherPos) const override;

	bool isCollidingWithSphere(const DirectX::XMFLOAT3& spherePos, const DirectX::XMFLOAT3& otherPos, float radius)const override;
	bool isCollidingWithPlane(const DirectX::XMFLOAT3& planePos, const DirectX::XMFLOAT3& planeNormal) const override;
};

