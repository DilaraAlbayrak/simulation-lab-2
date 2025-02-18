#pragma once
#include "Scenario.h"

class Bouncing : public Scenario
{
public:
	Bouncing() = default;
	~Bouncing() = default;
	void onLoad() override;
	void onUnload() override;
	void onUpdate(float dt = 0.016) override;
	void ImGuiMainMenu() override;

	ScenarioType getScenarioType() const override
	{
		return ScenarioType::BOUNCING;
	}
};

