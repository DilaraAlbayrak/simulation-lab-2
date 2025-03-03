#include "Colliding.h"
#include "Sphere.h"
#include "Plane.h"
#include <memory>

void Colliding::onLoad()
{
    OutputDebugString(L">>>>>>>>>> Colliding::onLoad\n");

	// Create a plane default position, rotation, and scale
	auto plane = std::make_unique<PhysicsObject>(std::make_unique<Plane>(), true);
    plane->LoadModel("plane.sjg");
	addPhysicsObject(std::move(plane));

    // Create two spheres
	auto sphere = std::make_unique<PhysicsObject>(
		std::make_unique<Sphere>(
			DirectX::XMFLOAT3(-3.5f, -2.5f, 0.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 
			DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f)
		)
	);
    sphere->LoadModel("sphere.sjg");
	sphere->setColliding(true);
	sphere->setVelocity({ 1.0f, 0.0f, 0.0f });
	sphere->setAngularVelocity({ 0.0f, 1.0f, 0.0f });
	ConstantBuffer cb = sphere->getConstantBuffer();
	cb.LightColour = { 0.2f, 0.6f, 0.2f, 1.0f };  // Green
	cb.DarkColour = { 0.3f, 0.1f, 0.3f, 1.0f };  
	sphere->setConstantBuffer(cb);
	sphere->setGravity(false);
	addPhysicsObject(std::move(sphere));

	// true for static object (non-moving)
	auto stationarySphere = std::make_unique<PhysicsObject>(
		std::make_unique<Sphere>(
			DirectX::XMFLOAT3(3.5f, -2.5f, 0.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f)
		), true
	);
	stationarySphere->LoadModel("sphere.sjg");
	stationarySphere->setColliding(true);
	cb = stationarySphere->getConstantBuffer();
	cb.LightColour = { 0.3f, 0.1f, 0.3f, 1.0f };  
	cb.DarkColour = { 0.3f, 0.1f, 0.3f, 1.0f };
	stationarySphere->setConstantBuffer(cb);
	addPhysicsObject(std::move(stationarySphere));

	setGravity(false);

	initObjects();
}

void Colliding::onUnload()
{
    OutputDebugString(L">>>>>>>>>> Colliding::onUnload\n");

	unloadScenario();
}

void Colliding::onUpdate(float dt)
{
	updateMovement(dt);
}

void Colliding::ImGuiMainMenu()
{
	applySharedGUI();
}
