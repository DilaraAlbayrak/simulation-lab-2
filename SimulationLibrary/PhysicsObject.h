#pragma once
#include "SJGLoader.h"

class PhysicsObject
{
private:
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 velocity;
    DirectX::XMFLOAT4X4 transformMatrix;

    std::vector<VertexData> vertices;
    std::vector<int> indices;

public:
    PhysicsObject(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 startVel)
        : position(startPos), velocity(startVel)
    {
        DirectX::XMStoreFloat4x4(&transformMatrix, DirectX::XMMatrixIdentity());
    }

    bool LoadModel(const std::string& filename)
    {
        return SJGLoader::Load(filename, vertices, indices);
    }

    void Update(float deltaTime)
    {
        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;
        position.z += velocity.z * deltaTime;

        DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        DirectX::XMStoreFloat4x4(&transformMatrix, translation);
    }

    const std::vector<VertexData>& GetVertices() const { return vertices; }
    const std::vector<int>& GetIndices() const { return indices; }
    const DirectX::XMFLOAT4X4& GetTransformMatrix() const { return transformMatrix; }
};