#include "Colliding.h"

void Colliding::onLoad()
{
    OutputDebugString(L">>>>>>>>>> Colliding::onLoad\n");

    // Create a plane
    auto plane = std::make_unique<PhysicsObject>(
        DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    plane->LoadModel("plane.sjg");
    physicsObjects.push_back(std::move(plane));

    // Create two spheres
    auto sphere1 = std::make_unique<PhysicsObject>(
        DirectX::XMFLOAT3(-1.0f, 0.5f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    sphere1->LoadModel("sphere.sjg");
    physicsObjects.push_back(std::move(sphere1));

    auto sphere2 = std::make_unique<PhysicsObject>(
        DirectX::XMFLOAT3(1.0f, 0.5f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    sphere2->LoadModel("sphere.sjg");
    physicsObjects.push_back(std::move(sphere2));
}

void Colliding::onUnload()
{
    OutputDebugString(L">>>>>>>>>> Colliding::onUnload\n");
    physicsObjects.clear();
}

void Colliding::onUpdate(float dt)
{
    // Simulate movement (if needed)
    for (auto& obj : physicsObjects)
    {
        obj->Update(dt);  // Assuming a fixed timestep for now
    }
}

void Colliding::renderObjects(ID3D11DeviceContext* context)
{
    for (auto& obj : physicsObjects)
    {
        // Implement rendering logic using the context
    }
}

void Colliding::ImGuiMainMenu()
{
}
