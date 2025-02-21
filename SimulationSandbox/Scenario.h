#pragma once
#include "PhysicsObject.h"
#include "NotImplementedException .h"
#include "ShaderManager.h"

enum class ScenarioType {
	SCENE_OPENING,
	BOUNCING,
	COLLIDING
};

//struct ConstantBuffer
//{
//	DirectX::XMMATRIX mWorld;
//};
struct ConstantBuffer
{
	DirectX::XMMATRIX World;
	DirectX::XMFLOAT2 CheckerboardSize = { 0.5f, 0.5f }; // Adjust tile size
	DirectX::XMFLOAT4 LightColor = { 0.6f, 0.2f, 0.2f, 1.0f };  // White
	DirectX::XMFLOAT4 DarkColor = { 0.1f, 0.1f, 0.3f, 1.0f };  // Black
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
	virtual void initObjects(ID3D11Device* device, ID3D11DeviceContext* context, const std::wstring& shaderFile = L"Simulation.fx") = 0;
	virtual void renderObjects(ID3D11DeviceContext* context, float dt = 0.016) = 0;
	virtual ScenarioType getScenarioType() const = 0;
};

