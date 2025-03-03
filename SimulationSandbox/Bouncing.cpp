#include "Bouncing.h"
#include "Sphere.h"
#include "Plane.h"
#include <memory>

#include "Sphere.h"
#include <memory>

void Bouncing::onLoad()
{
	OutputDebugString(L">>>>>>>>>> Bouncing::onLoad\n");

	// Create a plane default position, rotation, and scale
	auto plane = std::make_unique<PhysicsObject>(std::make_unique<Plane>(), true);
	plane->setColliding(true);
	plane->LoadModel("plane.sjg");
	addPhysicsObject(std::move(plane));

	// Create a sphere
	auto sphere = std::make_unique<PhysicsObject>(
		std::make_unique<Sphere>(
			DirectX::XMFLOAT3(0.0f, 0.0f, 2.0f),
			DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f)
		)
	);
	sphere->LoadModel("sphere.sjg");
	sphere->setGravity(true);
	sphere->setColliding(true);
	ConstantBuffer cb = sphere->getConstantBuffer();
	cb.LightColour = { 0.2f, 0.6f, 0.2f, 1.0f };
	cb.DarkColour = { 0.3f, 0.1f, 0.3f, 1.0f };
	sphere->setConstantBuffer(cb);
	addPhysicsObject(std::move(sphere));

	setGravity(true);

	initObjects();
}

void Bouncing::onUnload()
{
	OutputDebugString(L">>>>>>>>>> Bouncing::onUnload\n");

	unloadScenario();
}

void Bouncing::onUpdate(float dt)
{
	updateMovement(dt);
}

void Bouncing::ImGuiMainMenu()
{
	applySharedGUI();
}
