#pragma once
#include "PhysicsObject.h"
#include "NotImplementedException .h"
#include "ShaderManager.h"

//struct ConstantBuffer
//{
//	DirectX::XMMATRIX mWorld;
//};

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
	virtual void initObjects(ID3D11Device* device, ID3D11DeviceContext* context, const std::wstring& shaderFile = L"Simulation.fx") = 0;
	virtual void renderObjects(ID3D11DeviceContext* context, float dt = 0.016) = 0;
};

