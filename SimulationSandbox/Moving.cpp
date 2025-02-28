#include "Moving.h"
#include <atlbase.h>
#include "Sphere.h"
#include "Plane.h"
#include <memory>

void Moving::onLoad()
{
	OutputDebugString(L">>>>>>>>>> Moving::onLoad\n");

	// Create a plane
	auto plane = std::make_unique<PhysicsObject>(std::make_unique<Plane>(), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), true);
	plane->LoadModel("plane.sjg");
	addPhysicsObject(std::move(plane));

	// Create a sphere
	auto sphere = std::make_unique<PhysicsObject>(std::make_unique<Sphere>(), DirectX::XMFLOAT3(-2.5f, -2.5f, 0.0f));
	sphere->LoadModel("sphere.sjg");
	sphere->setVelocity({ 1.0f, 1.0f, 0.0f });
	sphere->setAngularVelocity({ 1.0f, 1.0f, 0.0f });
	sphere->setGravity(false);
	ConstantBuffer cb = sphere->getConstantBuffer();
	cb.LightColour = { 0.2f, 0.6f, 0.2f, 1.0f };  // Green
	cb.DarkColour = { 0.3f, 0.1f, 0.3f, 1.0f };
	sphere->setConstantBuffer(cb);
	addPhysicsObject(std::move(sphere));

	initObjects();
}

void Moving::onUnload()
{
	OutputDebugString(L">>>>>>>>>> Moving::onUnload\n");

	unloadScenario();
}

void Moving::onUpdate(float dt)
{
	updateMovement(dt);
}

void Moving::ImGuiMainMenu()
{
	applySharedGUI();
}
