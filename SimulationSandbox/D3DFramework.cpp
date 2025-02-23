#include "D3DFramework.h"
#include <directxcolors.h>
#include <vector>
#include "Resource.h"
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

std::unique_ptr<D3DFramework> D3DFramework::_instance = std::make_unique<D3DFramework>();
float D3DFramework::time = 0.0f;

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK D3DFramework::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message) {
	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT D3DFramework::initWindow(HINSTANCE hInstance, int nCmdShow) {
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, reinterpret_cast<LPCTSTR>(IDI_SIMULATION));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"Starter Template";
	wcex.hIconSm = LoadIcon(wcex.hInstance, reinterpret_cast<LPCTSTR>(IDI_SIMULATION));
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	_hInst = hInstance;
	RECT rc = { 0, 0, 800, 600 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_hWnd = CreateWindow(L"Starter Template", L"Simulation Sandbox",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!_hWnd)
		return E_FAIL;

	ShowWindow(_hWnd, nCmdShow);

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT D3DFramework::initDevice()
{
	HRESULT hr = static_cast<HRESULT>(S_OK);

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Supported driver types
	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	auto numDriverTypes = ARRAYSIZE(driverTypes);

	// Supported feature levels
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	auto numFeatureLevels = static_cast<UINT>(ARRAYSIZE(featureLevels));

	// Attempt to create the device and context
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex) {
		_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(
			nullptr, _driverType, nullptr, D3D11_CREATE_DEVICE_DEBUG,
			featureLevels, numFeatureLevels, D3D11_SDK_VERSION,
			&_pd3dDevice, &_featureLevel, &_pImmediateContext);

		if (hr == static_cast<HRESULT>(E_INVALIDARG)) {
			// Retry without D3D_FEATURE_LEVEL_11_1 if unsupported
			hr = D3D11CreateDevice(
				nullptr, _driverType, nullptr, D3D11_CREATE_DEVICE_DEBUG,
				&featureLevels[1], numFeatureLevels - 1, D3D11_SDK_VERSION,
				&_pd3dDevice, &_featureLevel, &_pImmediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
		return hr;

	// Obtain DXGI factory
	CComPtr<IDXGIFactory1> dxgiFactory;
	{
		CComPtr<IDXGIDevice> dxgiDevice;
		hr = _pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr)) {
			CComPtr<IDXGIAdapter> adapter;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr)) {
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
			}
		}
	}
	if (FAILED(hr))
		return hr;

	// Create swap chain
	CComPtr<IDXGIFactory2> dxgiFactory2;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));

	// DirectX 11.1 or later
	hr = _pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&_pd3dDevice1));
	if (SUCCEEDED(hr)) {
		static_cast<void>(_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&_pImmediateContext1)));
	}

	DXGI_SWAP_CHAIN_DESC1 sd{};
	sd.Width = _windowWidth;
	sd.Height = _windowHeight;
	sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;

	hr = dxgiFactory2->CreateSwapChainForHwnd(_pd3dDevice, _hWnd, &sd, nullptr, nullptr, &_swapChain1);
	if (SUCCEEDED(hr)) {
		hr = _swapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&_swapChain));
	}

	// Disable Alt+Enter fullscreen shortcut
	dxgiFactory->MakeWindowAssociation(_hWnd, DXGI_MWA_NO_ALT_ENTER);

	if (FAILED(hr))
		return hr;

	// Create render target view
	CComPtr<ID3D11Texture2D> pBackBuffer;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return hr;

	hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
	if (FAILED(hr))
		return hr;

	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView.p, nullptr);

	// Setup viewport
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(_windowWidth);
	vp.Height = static_cast<FLOAT>(_windowHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_pImmediateContext->RSSetViewports(1, &vp);

	// Create constant buffer for camera matrices
	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.ByteWidth = sizeof(ConstantBufferCamera);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = _pd3dDevice->CreateBuffer(&cbDesc, nullptr, &_cameraConstantBuffer);
	if (FAILED(hr)) {
		return hr;
	}

	// Initialize the camera
	initCamera();
	initImGui();

	return S_OK;
}

void D3DFramework::initCamera()
{
	_camera.eye = XMVectorSet(0.0f, -3.0f, 0.0f, 0.0f);
	_camera.at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	_camera.up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	_camera.zoom = 1.0f;
	_camera.updateViewProjection();
}

void D3DFramework::initImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(_hWnd);
	ImGui_ImplDX11_Init(_pd3dDevice, _pImmediateContext);
}

void D3DFramework::renderImGui() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Scenario"))
		{
			if (ImGui::MenuItem("Bouncing", nullptr, false)) {
				setScenario(std::make_unique<Bouncing>());
			}
			if (ImGui::MenuItem("Colliding", nullptr, false)) {
				setScenario(std::make_unique<Colliding>());
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::Begin("Colour Picker");

	float colour[3] = { _bgColour.x, _bgColour.y, _bgColour.z };

	ImGui::Text("Set background colour:");
	if (ImGui::ColorEdit3("Colour", colour))
	{
		DirectX::XMFLOAT4 newColour = { colour[0], colour[1], colour[2], 1.0f };
		{
			_bgColour = newColour;
		}
	}


	ImGui::End();

	//if (_scenarioType != ScenarioType::SCENE_OPENING)
	{
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Camera")) {
				if (ImGui::MenuItem("Reset", nullptr, false, true)) {
					initCamera();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		static float cameraAngle = 0.0f; 
		ImGui::Begin("Camera Controls");
		ImGui::SliderAngle("Rotation", &cameraAngle, 0.0f, 360.0f); 
		ImGui::End();

		_camera.rotateAroundOrigin(cameraAngle);
	}

	if (_scenario) {
		_scenario->ImGuiMainMenu();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
D3DFramework::~D3DFramework() {
	try {
		if (_pImmediateContext)
			_pImmediateContext->ClearState();
	}
	catch (...) {

	}
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void D3DFramework::render()
{
	static float currentTime = 0.0f;
	// Update our time	
	if (_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		time += static_cast<float>(XM_PI) * 0.0125f;
	}
	else
	{
		static ULONGLONG timeStart = 0;
		const ULONGLONG timeCur = GetTickCount64();
		if (timeStart == 0)
			timeStart = timeCur;
		time = (timeCur - timeStart) / 1000.0f;
	}
	deltaTime = (time - currentTime) * deltaTimeFactor;
	currentTime = time;

	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView.p, nullptr);
	//
	// Clear the back buffer
	//
	float clearColour[4] = { _bgColour.x, _bgColour.y, _bgColour.z, _bgColour.w };
	_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, clearColour);

	// Pass camera matrices to the constant buffer
	const ConstantBufferCamera cbc{
		DirectX::XMMatrixTranspose(_camera.view),
		DirectX::XMMatrixTranspose(_camera.projection),
		{
			DirectX::XMVectorGetX(_camera.eye),
			DirectX::XMVectorGetY(_camera.eye),
			DirectX::XMVectorGetZ(_camera.eye),
			time
		}
	};

	//if (_scenarioType == ScenarioType::COLLIDING)
	
		/*auto* collidingScene = dynamic_cast<Colliding*>(_scenario.get());
		if (collidingScene)
		{
			collidingScene->renderObjects(_pImmediateContext);
		}*/
	if (_scenario)
		_scenario->renderObjects(_pImmediateContext);
	
	_pImmediateContext->UpdateSubresource(_cameraConstantBuffer, 0, nullptr, &cbc, 0, 0);
	_pImmediateContext->VSSetConstantBuffers(0, 1, &_cameraConstantBuffer.p); // register b0

	renderImGui();
	_swapChain->Present(1, 0);
}