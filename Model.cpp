#include "Model.h"



Model::Model()
{
	this->vertexBuffer	= nullptr;
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
	v0.Color = DirectX::XMFLOAT4(0, 1, 0, 1); //green

	v1.Pos = DirectX::XMFLOAT3(1, 0, 0);
	v1.Color = DirectX::XMFLOAT4(1, 0, 0, 1); //red

	v2.Pos = DirectX::XMFLOAT3(-1, 0, 0);
	v2.Color = DirectX::XMFLOAT4(0, 0, 1, 1); //blue

	this->vertexData1.push_back(v0);
	this->vertexData1.push_back(v1);
	this->vertexData1.push_back(v2);
}

void Model::initializeTriangle(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext)
{
	HRESULT result;

	this->generateTriangle();

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(Vertex1) * this->vertexData1.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = false;
	desc.StructureByteStride = 0;

	result = gDevice->CreateBuffer(&desc, nullptr, &this->vertexBuffer);



	D3D11_MAPPED_SUBRESOURCE mappedData;
	
	result = gDeviceContext->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	Vertex1* v = reinterpret_cast<Vertex1*>(mappedData.pData);

	UINT size = this->vertexData1.size();
	for (UINT i = 0; i < size; i++)
	{
		v[i] = this->vertexData1.at(i);
	}

	gDeviceContext->Unmap(this->vertexBuffer, 0);

}

ID3D11Buffer* Model::getVertexBuffer() const
{
	return this->vertexBuffer;
}

void Model::shutdown()
{
	if (this->vertexBuffer != nullptr)
	{
		this->vertexBuffer->Release();
		this->vertexBuffer = nullptr;
	}


}


