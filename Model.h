#pragma once
#include "GlobalData.h"
#include <vector>

/*
The model class has everything in local space
*/

class Model
{
private:
	DirectX::XMMATRIX worldMatrix;
	std::vector<Vertex1>vertexData1;

	ID3D11Buffer* vertexBuffer;
	
public:
	Model();
	virtual ~Model();

	void generateTriangle();
	void initializeTriangle(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, const DirectX::XMFLOAT3& pos);

	ID3D11Buffer* getVertexBuffer() const;
	DirectX::XMMATRIX getWorldModel() const;

	void shutdown();
	
	//load from file
	//initialize
};

