#pragma once
#include "GlobalData.h"
#include <vector>

/*
The model class has everything in local space
*/

class Model
{
private:
	DirectX::XMFLOAT3 worldPos;
	DirectX::XMMATRIX worldMatrix;

	DirectX::XMMATRIX scaleMatrix;
	DirectX::XMMATRIX rotationMatrix;
	DirectX::XMMATRIX translationMatrix;

	std::vector<Vertex1>vertexData;

	ID3D11Buffer* vertexBuffer;
	
	float passiveSpinning;
	bool isSpinning;
	
public:
	Model();
	virtual ~Model();

	void generateTriangle();
	void initializeTriangle(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, const DirectX::XMFLOAT3& pos);

	ID3D11Buffer* getVertexBuffer() const;
	DirectX::XMMATRIX getWorldModel() const;

	void setUniformScale(const float& scalar);
	void rotateModelY(const float& degree);
	void spinnY(const float& degree);

	void update();

	void shutdown();
	
	//load from file
	//initialize
};

