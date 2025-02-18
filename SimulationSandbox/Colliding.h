#pragma once
#include "Scenario.h"
#include <memory>

class Colliding : public Scenario
{
private:
	std::vector<std::unique_ptr<PhysicsObject>> physicsObjects;

public:
	Colliding() = default;
	~Colliding() = default;

	void onLoad() override;
	void onUnload() override;
	void onUpdate(float dt = 0.016) override;
	void ImGuiMainMenu() override;

	ScenarioType getScenarioType() const override
	{
		return ScenarioType::COLLIDING;
	}

	void renderObjects(ID3D11DeviceContext* context);
};

