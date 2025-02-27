#include "Colliding.h"
#include <atlbase.h>
#include "Sphere.h"
#include "Plane.h"

void Colliding::onLoad()
{
    OutputDebugString(L">>>>>>>>>> Colliding::onLoad\n");

    // Create a plane
    auto plane = std::make_unique<PhysicsObject>(std::make_unique<Plane>(), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), true);
    plane->LoadModel("plane.sjg");
	addPhysicsObject(std::move(plane));

    // Create two spheres
    auto sphere = std::make_unique<PhysicsObject>(std::make_unique<Sphere>(), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    sphere->LoadModel("sphere.sjg");
	ConstantBuffer cb = sphere->getConstantBuffer();
	cb.LightColour = { 0.2f, 0.6f, 0.2f, 1.0f };  // Green
	cb.DarkColour = { 0.3f, 0.1f, 0.3f, 1.0f };  
	sphere->setConstantBuffer(cb);
	addPhysicsObject(std::move(sphere));

	initObjects();
}

void Colliding::onUnload()
{
    OutputDebugString(L">>>>>>>>>> Colliding::onUnload\n");

	unloadScenario();
}

void Colliding::onUpdate(float dt)
{
	for (auto& obj : getPhysicsObjects())
	{
		if (obj->getStaticInfo()) continue;
		// Update physics
		DirectX::XMFLOAT3 force = { 0.0f, -9.8f, 0.0f };  // Gravity
		//obj->applyForce(force);
		// Update object
		//obj->Update(dt);
	}
}

void Colliding::ImGuiMainMenu()
{
	applySharedGUI();
}
