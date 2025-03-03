#pragma once
#include <DirectXMath.h>

class Sphere;
class Plane;

class Collider
{
private:
	DirectX::XMFLOAT3 _position = { 0.0f, 0.0f, 0.0f };   
	DirectX::XMFLOAT3 _rotation = { 0.0f, 0.0f, 0.0f };   
	DirectX::XMFLOAT3 _scale = { 1.0f, 1.0f, 1.0f };     

public:
	Collider(DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f }, DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f }, DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f }) :
		_position(position), _rotation(rotation), _scale(scale)
	{
		if (_scale.x < 0.001f) _scale.x = 1.0f;
		if (_scale.y < 0.001f) _scale.y = 1.0f;
		if (_scale.z < 0.001f) _scale.z = 1.0f;
	}

	virtual ~Collider() = default;

	virtual bool isColliding(const Collider& other) const = 0;

	virtual bool isCollidingWithSphere(const Collider& other) const = 0;
	virtual bool isCollidingWithPlane(const Collider& other) const = 0;

	DirectX::XMMATRIX updateWorldMatrix() const;

	DirectX::XMFLOAT3 getPosition() const { return _position; }
	DirectX::XMFLOAT3 getRotation() const { return _rotation; }
	DirectX::XMFLOAT3 getScale() const { return _scale; }
	
	void setPosition(DirectX::XMFLOAT3 position) { _position = position; }
	void setRotation(DirectX::XMFLOAT3 rotation) { _rotation = rotation; }
	void setScale(DirectX::XMFLOAT3 scale) { _scale = scale; }

	void incrementPosition(DirectX::XMFLOAT3 increment) { _position.x += increment.x; _position.y += increment.y; _position.z += increment.z; }
	void incrementRotation(DirectX::XMFLOAT3 increment) { _rotation.x += increment.x; _rotation.y += increment.y; _rotation.z += increment.z; }
	void incrementScale(DirectX::XMFLOAT3 increment) { _scale.x += increment.x; _scale.y += increment.y; _scale.z += increment.z; }
};
