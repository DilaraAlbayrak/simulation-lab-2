#pragma once
#include "PhysicsObject.h"
#include "NotImplementedException .h"
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>

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
	virtual void onUpdate(float dt = 0.016) = 0;
	virtual void ImGuiMainMenu() = 0;
	virtual ScenarioType getScenarioType() const = 0;
};

