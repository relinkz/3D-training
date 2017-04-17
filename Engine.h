#pragma once
#include <windows.h>
#include"GlobalData.h"
#include "Model.h"
#include "Camera.h"
#include "GameTimer.h"
#include "inputHandler.h"

class Engine
{
private:
	struct dynamicResource
	{
		DirectX::XMMATRIX worldMatrix;
		DirectX::XMMATRIX viewMatrix;
		DirectX::XMMATRIX projectionMatrix;
	};
private:
	//This interface sets renderstates, bind resourses to the graphics pipeline, and issues rendering commands
	ID3D11DeviceContext* gDeviceContext;
	//Checks feature support and allocating resources
	ID3D11Device* gDevice;

	Camera* m_camera;

	std::vector<Model*> m_worldObj;

	//check 4x MSAA quality support
	bool enable4xMSAA; //false by default
	UINT g4xMsaaQuality;

	//interface hanling the front and backbuffer
	IDXGISwapChain* gSwapChain;

	ID3D11RenderTargetView* gRenderTargetView; //need this here?

	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* matrixBuffer;

	ID3D11InputLayout* inputLayout;
	
public:
	Engine();
	virtual ~Engine();

	bool initialize(HWND* window);
	void shutdown();
	void update();

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();

	void fillCBuffers(DirectX::XMMATRIX modelWorldMatrix, const Camera &gameCamera);
	void drawObject(Model &toDraw);

	void addModel(Model* ptr);
};

