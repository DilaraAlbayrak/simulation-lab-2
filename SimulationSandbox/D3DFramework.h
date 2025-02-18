#pragma once
// Simulation template, based on the Microsoft DX11 tutorial 04

#include <atlbase.h>
#include <fstream>

#include "SceneOpening.h"
#include "Bouncing.h"
#include "Colliding.h"

using namespace DirectX;

#define COMPILE_CSO

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};


struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

class D3DFramework final {

	HINSTANCE _hInst = nullptr;
	HWND _hWnd = nullptr;
	D3D_DRIVER_TYPE _driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL _featureLevel = D3D_FEATURE_LEVEL_11_1;
	CComPtr <ID3D11Device> _pd3dDevice;
	CComPtr <ID3D11Device1> _pd3dDevice1;
	CComPtr <ID3D11DeviceContext> _pImmediateContext;
	CComPtr <ID3D11DeviceContext1> _pImmediateContext1;
	CComPtr <IDXGISwapChain1> _swapChain;
	CComPtr <IDXGISwapChain1> _swapChain1;
	CComPtr <ID3D11RenderTargetView> _pRenderTargetView;
	CComPtr <ID3D11VertexShader> _pVertexShader;
	CComPtr <ID3D11PixelShader> _pPixelShader;
	CComPtr <ID3D11InputLayout> _pVertexLayout;
	CComPtr <ID3D11Buffer> _pVertexBuffer;
	CComPtr <ID3D11Buffer> _pIndexBuffer;
	CComPtr <ID3D11Buffer> _pConstantBuffer;
	XMMATRIX _World = {};
	XMMATRIX _View = {};
	XMMATRIX _Projection = {};

	XMFLOAT4 _bgColour = { 0.2f, 0.2f, 0.6f, 1.0f };

	// scenario stuffs
	ScenarioType _scenarioType = ScenarioType::SCENE_OPENING;
	std::unique_ptr<Scenario> _scenario;

	static std::unique_ptr<D3DFramework> _instance;

	void initImGui();
	void renderImGui();

public:

	D3DFramework()
	{
		_scenario = std::make_unique<SceneOpening>(); 
		_scenarioType = _scenario->getScenarioType();
		auto* openingScene = dynamic_cast<SceneOpening*>(_scenario.get());
		if (openingScene)
		{
			openingScene->setBgColour(_bgColour);
			openingScene->setOnColorChangeCallback([this](const DirectX::XMFLOAT4& colour) {
				_bgColour = colour;
				});
			openingScene->onLoad();
		}
	}
	D3DFramework(D3DFramework&) = delete;
	D3DFramework(D3DFramework&&) = delete;
	D3DFramework operator=(const D3DFramework&) = delete;
	D3DFramework operator=(const D3DFramework&&) = delete;
	~D3DFramework();

	static D3DFramework& getInstance() { return *_instance; }

	// callback function that Windows calls whenever an event occurs for the window (e.g., mouse clicks, key presses)
	// Windows expects this function to have a specific signature and does not pass an instance of the class to it
	static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HRESULT initWindow(HINSTANCE hInstance, int nCmdShow);
	// utility function that doesn't require an instance of the class
	static HRESULT compileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT initDevice();
	void render();

	HWND getWindowHandle() const { return _hWnd; }
	ID3D11Device* getDevice() const { return _pd3dDevice; }
	ID3D11DeviceContext* getDeviceContext() const { return _pImmediateContext; }

	void setScenario(std::unique_ptr<Scenario> scenario, ScenarioType scenarioType)
	{
		if (_scenarioType == scenarioType) return;

		_scenario.get()->onUnload();
		_scenario = std::move(scenario);
		_scenarioType = _scenario->getScenarioType();
		_scenario.get()->onLoad();
	}

	void setBackgroudColor(const XMFLOAT4& colour) { _bgColour = colour; }
	XMFLOAT4 getBackgroundColor() const { return _bgColour; }
};