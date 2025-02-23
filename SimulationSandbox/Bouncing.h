#pragma once
#include "Scenario.h"

class Bouncing : public Scenario
{
protected:

public:
	Bouncing() = default;
	~Bouncing() = default;
	void onLoad() override;
	void onUnload() override;
	void onUpdate(float dt = 0.016) override;
	void ImGuiMainMenu() override;

	void initObjects(ID3D11Device* device, ID3D11DeviceContext* context, const std::wstring& shaderFile = L"Simulation.fx") override;
	void renderObjects(ID3D11DeviceContext* context, float dt = 0.016) override;
};

