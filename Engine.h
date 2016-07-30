#pragma once
#include <windows.h>
#include"GlobalData.h"

class Engine
{
private:
	//This interface sets renderstates, bind resourses to the graphics pipeline, and issues rendering commands
	ID3D11DeviceContext* gDeviceContext;
	//Checks feature support and allocating resources
	ID3D11Device* gDevice;

	//check 4x MSAA quality support
	bool enable4xMSAA; //false by default
	UINT g4xMsaaQuality;

	IDXGISwapChain* gSwapChain;
	ID3D11RenderTargetView* gRenderTargetView; //need this here?

	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;
	
public:
	Engine();
	virtual ~Engine();

	bool initialize(HWND* window);
	void shutdown();
};

