#pragma once
#include "Scenario.h"
#include <memory>

class Moving : public Scenario
{
public:
	//Colliding(const CComPtr <ID3D11Device> &pDevice, const CComPtr <ID3D11DeviceContext> &pContext) : device(pDevice), context(pContext) {}
	Moving(const CComPtr <ID3D11Device>& pDevice, const CComPtr <ID3D11DeviceContext>& pContext) : Scenario(pDevice, pContext) {}
	~Moving() = default;

	void onLoad() override;
	void onUnload() override;
	void onUpdate(float dt = 0.016) override;
	void ImGuiMainMenu() override;
};

