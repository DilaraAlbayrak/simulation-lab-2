#pragma once
#include "SJGLoader.h"

struct ConstantBuffer
{
    DirectX::XMMATRIX World;
    DirectX::XMFLOAT4 LightColour = { 0.8f, 0.8f, 0.8f, 1.0f };  // White
    DirectX::XMFLOAT4 DarkColour = { 0.2f, 0.2f, 0.2f, 1.0f };  // Black
    DirectX::XMFLOAT2 CheckerboardSize = { 1.0f, 1.0f }; // Adjust tile size
	DirectX::XMFLOAT2 Padding = { 0.0f, 0.0f };
};

class PhysicsObject
{
private:
    DirectX::XMFLOAT3 position;   // Position in world space
    DirectX::XMFLOAT3 rotation;   // Rotation (pitch, yaw, roll)
    DirectX::XMFLOAT3 scale;      // Scale factors

    std::vector<Vertex> vertices;
    std::vector<int> indices;

	ConstantBuffer constantBuffer;

public:
    PhysicsObject(DirectX::XMFLOAT3 startPos = {0.0f, 0.0f, 0.0f}, DirectX::XMFLOAT3 startRot = { 0.0f, 0.0f, 0.0f }, DirectX::XMFLOAT3 startScale = { 1.0f, 1.0f, 1.0f })
        : position(startPos), rotation(startRot), scale(startScale)
    {
        updateWorldMatrix();
    }

    bool LoadModel(const std::string& filename)
    {
        return SJGLoader::Load(filename, vertices, indices);
    }

    void Update(float deltaTime)
    {
        position.x += deltaTime;
        position.y += deltaTime;
        position.z += deltaTime;

        updateWorldMatrix();
    }

    void updateWorldMatrix()
    {
        DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
        DirectX::XMMATRIX rotationMatrix =
            DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
        DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

        DirectX::XMMATRIX worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;
		constantBuffer.World = DirectX::XMMatrixTranspose(worldMatrix);
    }

    const std::vector<Vertex>& getVertices() const { return vertices; }
    const std::vector<int>& getIndices() const { return indices; }
    const DirectX::XMMATRIX& getTransformMatrix() const { return constantBuffer.World; }
	const ConstantBuffer getConstantBuffer() const { return constantBuffer; }

    void setPosition(const DirectX::XMFLOAT3& newPos) { position = newPos; updateWorldMatrix(); }
    void setRotation(const DirectX::XMFLOAT3& newRot) { rotation = newRot; updateWorldMatrix(); }
    void setScale(const DirectX::XMFLOAT3& newScale) { scale = newScale; updateWorldMatrix(); }
	void setConstantBuffer(const ConstantBuffer& cb) { constantBuffer = cb; }
};
