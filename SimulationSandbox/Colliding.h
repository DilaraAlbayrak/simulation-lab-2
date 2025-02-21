#pragma once
#include "Scenario.h"
#include <memory>

class Colliding : public Scenario
{
private:
	std::vector<std::unique_ptr<PhysicsObject>> physicsObjects;

	std::vector<CComPtr<ID3D11Buffer>> vertexBuffers;
	std::vector<CComPtr<ID3D11Buffer>> indexBuffers;
	std::vector<CComPtr<ID3D11InputLayout>> inputLayouts;
	std::vector<CComPtr<ID3D11Buffer>> constantBuffers;
	CComPtr<ID3D11VertexShader> vertexShader;
	CComPtr<ID3D11PixelShader> pixelShader;
	CComPtr<ID3DBlob> vertexShaderBlob;
	CComPtr<ID3DBlob> pixelShaderBlob;
	std::vector<UINT> indexCounts;

public:
	Colliding() = default;
	~Colliding() = default;

	void onLoad() override;
	void onUnload() override;
	void onUpdate(float dt = 0.016) override;
	void ImGuiMainMenu() override;

	void initObjects(ID3D11Device* device, ID3D11DeviceContext* context, const std::wstring& shaderFile = L"Simulation.fx") override;
	void renderObjects(ID3D11DeviceContext* context, float dt = 0.016) override;

	ScenarioType getScenarioType() const override
	{
		return ScenarioType::COLLIDING;
	}
};

