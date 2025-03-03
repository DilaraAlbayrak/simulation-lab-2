#pragma once
#include "Collider.h"

class Sphere : public Collider
{
private:
	float _radius = 1.0f;
public:
	Sphere(
		DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f },
		DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f },
		DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f })
		: Collider(position, rotation, scale) { _radius = scale.x; }
	~Sphere() = default;
	bool isColliding(const Collider& other) const override;

	bool isCollidingWithSphere(const Collider& other) const override;
    bool isCollidingWithPlane(const Collider& other) const override;

	float getRadius() const { return _radius; }
};

