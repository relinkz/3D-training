#pragma once
#include "GlobalData.h"
#include <vector>

class Model
{
private:
	std::vector<Vertex1>vertexData1;
	std::vector<Vertex2>vertexData2;

	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
public:
	Model();
	virtual ~Model();
};

