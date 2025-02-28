#pragma once
#include "Collider.h"

class Sphere : public Collider
{
private:
	float _radius = 1.0f;
public:
	Sphere(float radius = 1.0f) : _radius(radius) {}
	~Sphere() = default;
	bool isColliding(const Collider& other, const DirectX::XMFLOAT3& thisPos, const DirectX::XMFLOAT3& otherPos) const override;

	bool isCollidingWithSphere(const DirectX::XMFLOAT3& spherePos, float radius) const override;
    bool isCollidingWithPlane(const DirectX::XMFLOAT3& planePos, const DirectX::XMFLOAT3& planeNormal) const override;

	void setRadius(float radius) { _radius = radius; }
	float getRadius() const { return _radius; }
};

