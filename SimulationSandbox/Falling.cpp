#include "Falling.h"
#include "Sphere.h"
#include <memory>

void Falling::onLoad()
{
	OutputDebugString(L">>>>>>>>>> Falling::onLoad\n");

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
	ConstantBuffer cb = sphere->getConstantBuffer();
	cb.LightColour = { 0.2f, 0.6f, 0.2f, 1.0f };  
	cb.DarkColour = { 0.3f, 0.1f, 0.3f, 1.0f };
	sphere->setConstantBuffer(cb);
	addPhysicsObject(std::move(sphere));

	setGravity(true);

	initObjects();
}

void Falling::onUnload()
{
	OutputDebugString(L">>>>>>>>>> Falling::onUnload\n");

	unloadScenario();
}

void Falling::onUpdate(float dt)
{
	updateMovement(dt);
}

void Falling::ImGuiMainMenu()
{
	applySharedGUI();
}
