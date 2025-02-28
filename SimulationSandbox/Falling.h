#pragma once
#include "Scenario.h"

class Falling : public Scenario
{
public:
	Falling(const CComPtr <ID3D11Device>& pDevice, const CComPtr <ID3D11DeviceContext>& pContext) : Scenario(pDevice, pContext) {}
	~Falling() = default;

	void onLoad() override;
	void onUnload() override;
	void onUpdate(float dt = 0.016) override;
	void ImGuiMainMenu() override;
};

