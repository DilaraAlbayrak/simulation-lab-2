#pragma once
#include "NotImplementedException .h"
#include <windows.h>

enum class ScenarioType {
	SCENE_OPENING,
	BOUNCING,
	COLLIDING
};

class Scenario
{
protected:
	Scenario() = default;
public:
	virtual ~Scenario() = default;

	virtual void onLoad() = 0;
	virtual void onUnload() = 0;
	virtual void onUpdate() = 0;
	virtual void ImGuiMainMenu() = 0;
	virtual ScenarioType getScenarioType() const = 0;
};

