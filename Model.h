#pragma once
#include "GlobalData.h"
#include <vector>

/*
The model class has everything in local space
*/

class Model
{
private:
	std::vector<Vertex1>vertexData1;

	ID3D11Buffer* vertexBuffer;
	
public:
	Model();
	virtual ~Model();

	void generateTriangle();
	void initializeTriangle(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext);

	ID3D11Buffer* getVertexBuffer() const;

	void shutdown();
	
	//load from file
	//initialize
};

