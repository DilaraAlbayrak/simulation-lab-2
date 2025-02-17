#pragma once
#include "Scenario.h"

class Colliding : public Scenario
{
public:
	Colliding() = default;
	~Colliding() = default;

	void onLoad() override;
	void onUnload() override;
	void onUpdate() override;
	void ImGuiMainMenu() override;

	ScenarioType getScenarioType() const override
	{
		return ScenarioType::COLLIDING;
	}
};

