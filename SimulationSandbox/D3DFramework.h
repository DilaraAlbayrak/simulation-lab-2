#pragma once
// Simulation template, based on the Microsoft DX11 tutorial 04

#include <atlbase.h>
#include <fstream>

#include "SceneOpening.h"
#include "Bouncing.h"
#include "Colliding.h"

using namespace DirectX;

#define COMPILE_CSO

constexpr UINT _windowWidth = 800;
constexpr UINT _windowHeight = 600;

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------

struct ConstantBufferCamera
{
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMVECTOR mEyePos;
};

enum CameraType
{
	ORTHOGRAPHIC,
	PERSPECTIVE
};

struct Camera
{
	XMVECTOR eye;
	XMVECTOR at;
	XMVECTOR up;
	XMMATRIX view = {};
	XMMATRIX projection = {};
	float zoom = 1.0f; // Default zoom level

	void updateViewProjection() {
		// Adjust field of view (FOV) based on zoom level
		if (zoom < 0.9f) zoom = 0.9f;
		if (zoom > 1.6f) zoom = 1.6f;

		const float fov = XM_PIDIV2 / zoom; // Zoom scales the FOV
		projection = XMMatrixPerspectiveFovLH(fov, 800.0f / 600.0f, 0.01f, 1000.0f);
		view = XMMatrixLookAtLH(eye, at, up);

		//setOrthographicProjection();
	}

	// temp
	std::string XMVectorToString(const DirectX::XMVECTOR& vec) {
		DirectX::XMFLOAT3 float3;
		DirectX::XMStoreFloat3(&float3, vec);
		return "X: " + std::to_string(float3.x) + " Y: " + std::to_string(float3.y) + " Z: " + std::to_string(float3.z);
	}

	void rotateAroundOrigin(float angle) {
		float radius = 3.0f; 

		float x = radius * sin(angle);
		float z = radius * cos(angle);

		eye = XMVectorSet(0.0f, -3.0f, z, 0.0f);
		at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); 
		up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); 

		/*OutputDebugStringA("--------------------\n");
		OutputDebugStringA(XMVectorToString(eye).c_str());
		OutputDebugStringA("\n");
		OutputDebugStringA(XMVectorToString(at).c_str());
		OutputDebugStringA("\n");
		OutputDebugStringA(XMVectorToString(up).c_str());
		OutputDebugStringA("\n");
		OutputDebugStringA(std::to_string(angle).c_str());
		OutputDebugStringA("\n");*/

		updateViewProjection();
	}

	void setOrthographicProjection()
	{
		eye = XMVectorSet(0.0f, 0.0f, 3.0f, 0.0f);
		at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		up = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		zoom = 1.0f;
		const float orthoWidth = 10.0f / zoom;
		const float orthoHeight = (_windowHeight * 1.0f / _windowWidth) * orthoWidth;

		projection = XMMatrixOrthographicLH(orthoWidth, orthoHeight, 0.01f, 1000.0f);

		view = XMMatrixLookAtLH(eye, at, up);
	}
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
	CComPtr <ID3D11Buffer> _cameraConstantBuffer;
	XMMATRIX _World = {};
	XMMATRIX _View = {};
	XMMATRIX _Projection = {};

	XMFLOAT4 _bgColour = { 0.2f, 0.2f, 0.6f, 1.0f };
	static float time;
	float deltaTime = 0.0f;
	float deltaTimeFactor = 1.0f;

	Camera _camera;

	// scenario stuffs
	ScenarioType _scenarioType = ScenarioType::SCENE_OPENING;
	std::unique_ptr<Scenario> _scenario;

	static std::unique_ptr<D3DFramework> _instance;

	void initImGui();
	void renderImGui();

	void initCamera();

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

		if (_scenarioType == ScenarioType::COLLIDING)
		{
			auto* collidingScene = dynamic_cast<Colliding*>(_scenario.get());
			if (collidingScene)
			{
				collidingScene->initObjects(_pd3dDevice, _pImmediateContext);
			}
		}
	}

	void setBackgroudColor(const XMFLOAT4& colour) { _bgColour = colour; }
	XMFLOAT4 getBackgroundColor() const { return _bgColour; }
};