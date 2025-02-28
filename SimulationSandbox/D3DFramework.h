#pragma once
// Simulation template, based on the Microsoft DX11 tutorial 04

#include <atlbase.h>
#include <fstream>

#include "Scenario.h"

using namespace DirectX;

#define COMPILE_CSO

constexpr UINT _windowWidth = 1200;
constexpr UINT _windowHeight = 900;

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
		projection = XMMatrixPerspectiveFovLH(fov, _windowWidth / _windowHeight, 0.01f, 1000.0f);
		view = XMMatrixLookAtLH(eye, at, up);

		//setOrthographicProjection();
	}

	// temp
	std::string XMVectorToString(const DirectX::XMVECTOR& vec) {
		DirectX::XMFLOAT3 float3;
		DirectX::XMStoreFloat3(&float3, vec);
		return "X: " + std::to_string(float3.x) + " Y: " + std::to_string(float3.y) + " Z: " + std::to_string(float3.z);
	}

	void rotateUp(float angle) {
		float radius = 3.0f; 

		float x = radius * sin(angle);
		float z = radius * cos(angle);

		eye = XMVectorSet(x, z, 3.0, 0.0f);
		at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); 
		up = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); 

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
		const float orthoWidth = 10.0f / zoom;  // Adjust width based on zoom
		const float orthoHeight = (_windowHeight / _windowWidth) * orthoWidth; // Maintain aspect ratio

		projection = XMMatrixOrthographicLH(orthoWidth, orthoHeight, 0.01f, 1000.0f);
		view = XMMatrixLookAtLH(eye, at, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
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

	XMFLOAT4 _bgColour = { 0.54f, 0.75f, 0.77f, 1.0f };
	float deltaTime = 0.0f;
	const float deltaTimeFactors[4] = {0.25f, 0.5f, 1.0f, 2.0f};
	float deltaTimeFactor = deltaTimeFactors[2];
	static float time;
	const float fixedTimesteps[4] = { 0.002f, 0.004f, 0.008f, 0.016f };
	float fixedTimestep = fixedTimesteps[1];
	const float maxSteps[4] = { 8, 4, 2, 1 };
	int numMaxStep = maxSteps[1];
	float cumulativeTime = 0.0f;

	Camera _camera;

	// scenario stuffs
	std::unique_ptr<Scenario> _scenario;

	static std::unique_ptr<D3DFramework> _instance;

	void initImGui();
	void renderImGui();

	void initCamera();

	void setScenario(std::unique_ptr<Scenario> scenario)
	{
		if (_scenario)
			_scenario.get()->onUnload();
		_scenario = std::move(scenario);
		_scenario.get()->onLoad();
	}

	void resetScenario()
	{
		if (!_scenario) return;

		_scenario.get()->onUnload();
		_scenario.get()->onLoad();

	}

public:

	D3DFramework() = default;
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
	HRESULT initDevice();
	void render();

	HWND getWindowHandle() const { return _hWnd; }
	ID3D11Device* getDevice() const { return _pd3dDevice; }
	ID3D11DeviceContext* getDeviceContext() const { return _pImmediateContext; }

	void setBackgroudColor(const XMFLOAT4& colour) { _bgColour = colour; }
	XMFLOAT4 getBackgroundColor() const { return _bgColour; }
};