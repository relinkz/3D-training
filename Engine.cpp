#include "Engine.h"
#include <atlbase.h>

Engine::Engine()
{
	this->gDeviceContext = nullptr;
	this->gDevice = nullptr;
	
	this->g4xMsaaQuality = 0;
	this->enable4xMSAA = false;

	this->gSwapChain = nullptr;
	this->gRenderTargetView = nullptr;

	this->mDepthStencilBuffer = nullptr;
	this->mDepthStencilView = nullptr;
}


Engine::~Engine()
{
}

bool Engine::initialize(HWND* window)
{
	HRESULT result;
	//*************************
	//Creating the interfaces**
	//*************************
	D3D_FEATURE_LEVEL fLevel;

	result = D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		D3D11_CREATE_DEVICE_DEBUG,
		0,
		0,
		D3D11_SDK_VERSION,
		&this->gDevice,
		&fLevel,
		&this->gDeviceContext
		);

	if (FAILED(result))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}
	if (fLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature level 11 unsupported",0 ,0);
		return false;
	}

	//*************************
	//4XMSAA Quality support***
	//*************************
	//with this technique, the back and depth buffer are rendered with
	//a resulotion 4 times bigger than the monitors resolution. When it's time
	//for the backbuffer to be presented (becoming "front buffer") the buffer is 
	//resolved (downsampled), taking the average of 4 pixels, smoothing edges.

	result = this->gDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &this->g4xMsaaQuality);
	if (this->g4xMsaaQuality < 0)
	{
		this->g4xMsaaQuality = 0;
	}

	//*************************
	//Swap chain            ***
	//*************************
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = CLIENT_WIDTH;
	sd.BufferDesc.Height = CLIENT_HEIGHT;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (this->enable4xMSAA)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = this->g4xMsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = *window;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;


	//haxxing required to avoid incluing libs, to get a factory
	IDXGIDevice* dxgiDevice = 0;
	result = this->gDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;
	result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	//getting the COM factory
	IDXGIFactory* dxgiFactory = 0;
	result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	//create the swapchain
	result = dxgiFactory->CreateSwapChain(this->gDevice, &sd, &this->gSwapChain);

	//realeasing the com object since they are no longer needed
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	//*************************
	//render target view    ***
	//*************************
	ID3D11Texture2D* backBuffer;
	this->gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	
	result = this->gDevice->CreateRenderTargetView(backBuffer, 0, &this->gRenderTargetView);

	//the getBuffer increases the com reference count, therefore releasing it,
	//but what happens whith the backbuffer??????
	backBuffer->Release();

	/*
	Depth stencil / z-buffer and view
	*/

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = CLIENT_WIDTH;
	depthStencilDesc.Height = CLIENT_HEIGHT;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	if (this->enable4xMSAA)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = this->g4xMsaaQuality;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	result = this->gDevice->CreateTexture2D(&depthStencilDesc, 0, &this->mDepthStencilBuffer);

	if (FAILED(result))
	{
		MessageBox(0, L"Creating Depth-buffer failed.", 0, 0);
		return false;
	}

	result = this->gDevice->CreateDepthStencilView(this->mDepthStencilBuffer, 0, &this->mDepthStencilView);

	if (FAILED(result))
	{
		MessageBox(0, L"Creating Depth-stencil view failed.", 0, 0);
		return false;
	}

	this->gDeviceContext->OMSetRenderTargets(1, &gRenderTargetView, this->mDepthStencilView);

	/*
	Set viewport
	*/
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(CLIENT_WIDTH);
	vp.Height = static_cast<float>(CLIENT_HEIGHT);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	this->gDeviceContext->RSSetViewports(1,&vp);

	return true;
}

void Engine::shutdown()
{
	if (this->gSwapChain != nullptr)
	{
		this->gSwapChain->Release();
		this->gSwapChain = nullptr;
	}

	if (this->gRenderTargetView != nullptr)
	{
		this->gRenderTargetView->Release();
		this->gRenderTargetView = nullptr;
	}

	if (this->mDepthStencilBuffer != nullptr)
	{
		this->mDepthStencilBuffer->Release();
		this->mDepthStencilBuffer = nullptr;
	}

	if (this->mDepthStencilView != nullptr)
	{
		this->mDepthStencilView->Release();
		this->mDepthStencilView = nullptr;
	}

	if (this->gDeviceContext != nullptr)
	{
		this->gDeviceContext->Release();
		this->gDeviceContext = nullptr;
	}

	if (this->gDevice != nullptr)
	{
		this->gDevice->Release();
		this->gDevice = nullptr;
	}
}
