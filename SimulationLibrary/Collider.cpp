#include "pch.h"
#include "Collider.h"

DirectX::XMMATRIX Collider::updateWorldMatrix() const
{
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	DirectX::XMMATRIX rotationMatrix =
		DirectX::XMMatrixRotationRollPitchYaw(_rotation.x, _rotation.y, _rotation.z);
	DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(_position.x, _position.y, _position.z);

	DirectX::XMMATRIX worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;
	return DirectX::XMMatrixTranspose(worldMatrix);
}