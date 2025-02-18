#pragma once
#include "Scenario.h"
#include <DirectXMath.h>
#include <functional>

// default scenario
class SceneOpening : public Scenario
{
private:
    DirectX::XMFLOAT4 _bgColour = { 0.2f, 0.2f, 0.6f, 1.0f };
    std::function<void(const DirectX::XMFLOAT4&)> _onColourChange;

public:
	SceneOpening() = default;
    ~SceneOpening() = default;

    void onLoad() override;
    void onUnload() override;
    void onUpdate(float dt = 0.016) override;
    void ImGuiMainMenu() override;

	ScenarioType getScenarioType() const override
	{
		return ScenarioType::SCENE_OPENING;
	}

    void setOnColorChangeCallback(std::function<void(const DirectX::XMFLOAT4&)> callback)
    {
        _onColourChange = callback;
    }

	void setBgColour(const DirectX::XMFLOAT4& colour)
	{
		_bgColour = colour;
	}
};