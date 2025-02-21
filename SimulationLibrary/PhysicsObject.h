#pragma once
#include "SJGLoader.h"

class PhysicsObject
{
private:
    DirectX::XMFLOAT3 position;   // Position in world space
    DirectX::XMFLOAT3 rotation;   // Rotation (pitch, yaw, roll)
    DirectX::XMFLOAT3 scale;      // Scale factors

    DirectX::XMFLOAT4X4 transformMatrix; // World matrix

    std::vector<Vertex> vertices;
    std::vector<int> indices;

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
        DirectX::XMStoreFloat4x4(&transformMatrix, worldMatrix);
    }

    const std::vector<Vertex>& getVertices() const { return vertices; }
    const std::vector<int>& getIndices() const { return indices; }
    const DirectX::XMFLOAT4X4& getTransformMatrix() const { return transformMatrix; }

    void setPosition(const DirectX::XMFLOAT3& newPos) { position = newPos; updateWorldMatrix(); }
    void setRotation(const DirectX::XMFLOAT3& newRot) { rotation = newRot; updateWorldMatrix(); }
    void setScale(const DirectX::XMFLOAT3& newScale) { scale = newScale; updateWorldMatrix(); }
};
