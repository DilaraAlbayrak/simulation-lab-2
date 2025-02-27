#pragma once
#include "Scenario.h"

class Bouncing : public Scenario
{
public:
	Bouncing(const CComPtr <ID3D11Device>& pDevice, const CComPtr <ID3D11DeviceContext>& pContext) : Scenario(pDevice, pContext) {}
	~Bouncing() = default;
	void onLoad() override;
	void onUnload() override;
	void onUpdate(float dt = 0.016) override;
	void ImGuiMainMenu() override;
};

