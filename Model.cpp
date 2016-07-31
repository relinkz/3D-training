#include "Model.h"



Model::Model()
{
	this->inputLayout	= nullptr;
	this->vertexBuffer	= nullptr;
	this->indexBuffer	= nullptr;
}


Model::~Model()
{
}

void Model::generateTriangle()
{
	Vertex1 v0;
	Vertex1 v1;
	Vertex1 v2;

	v0.Pos = DirectX::XMFLOAT3(0, 1, 0);
	v0.Color = DirectX::XMFLOAT4(0, 255, 0, 1); //green

	v1.Pos = DirectX::XMFLOAT3(1, 0, 0);
	v1.Color = DirectX::XMFLOAT4(255, 0, 0, 1); //red

	v2.Pos = DirectX::XMFLOAT3(-1, 0, 0);
	v2.Color = DirectX::XMFLOAT4(255, 0, 0, 1);	//red

	this->vertexData1.push_back(v0);
	this->vertexData1.push_back(v1);
	this->vertexData1.push_back(v2);
}


