#pragma once
#include "PhysicsObject.h"
#include "NotImplementedException .h"
#include "ShaderManager.h"

class Scenario
{
private:
	std::vector<std::unique_ptr<PhysicsObject>> physicsObjects;
	std::vector<CComPtr<ID3D11Buffer>> vertexBuffers;
	std::vector<CComPtr<ID3D11Buffer>> indexBuffers;
	std::vector<CComPtr<ID3D11InputLayout>> inputLayouts;
	std::vector<CComPtr<ID3D11Buffer>> constantBuffers;
	CComPtr <ID3D11Device> device;
	CComPtr <ID3D11DeviceContext> context;
	CComPtr<ID3D11VertexShader> vertexShader;
	CComPtr<ID3D11PixelShader> pixelShader;
	CComPtr<ID3DBlob> vertexShaderBlob;
	CComPtr<ID3DBlob> pixelShaderBlob;
	std::vector<UINT> indexCounts;

	int integrationMethod;
	bool applyGravity = false;

protected:
	Scenario(const CComPtr <ID3D11Device>& pDevice, const CComPtr <ID3D11DeviceContext>& pContext) : device(pDevice), context(pContext) {}
	void initObjects(const std::wstring& shaderFile = L"Simulation.fx");
	void unloadScenario();
	void applySharedGUI();

	const int getIntegrationMethod() const { return integrationMethod; }

public:
	virtual ~Scenario() = default;

	virtual void onLoad() = 0;
	virtual void onUnload() = 0;
	virtual void onUpdate(float dt = 0.016) = 0;
	virtual void ImGuiMainMenu() = 0;

	void renderObjects();

	std::vector<std::unique_ptr<PhysicsObject>>& getPhysicsObjects() { return physicsObjects; }
	void addPhysicsObject(std::unique_ptr<PhysicsObject> obj) { physicsObjects.push_back(std::move(obj)); }
};

