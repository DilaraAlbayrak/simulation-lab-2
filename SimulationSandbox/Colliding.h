#pragma once
#include "Scenario.h"
#include <memory>

class Colliding : public Scenario
{
public:
	Colliding(const CComPtr <ID3D11Device>& pDevice, const CComPtr <ID3D11DeviceContext>& pContext) : Scenario(pDevice, pContext) {}
	~Colliding() = default;

	void onLoad() override;
	void onUnload() override;
	void onUpdate(float dt = 0.016) override;
	void ImGuiMainMenu() override;
};

