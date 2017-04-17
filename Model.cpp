#include "Model.h"



Model::Model()
{
	this->vertexBuffer	= nullptr;
	this->worldMatrix = DirectX::XMMatrixIdentity();

	this->scaleMatrix = DirectX::XMMatrixIdentity();
	this->rotationMatrix = DirectX::XMMatrixIdentity();
	this->translationMatrix = DirectX::XMMatrixIdentity();

	this->isSpinning = false;
	this->passiveSpinning = 0.0f;

	this->scaleSpeed = 0;
	this->tempScale = 1.0f;
	this->isScaling = false;
	this->scaleState = false;
}


Model::~Model()
{
}

void Model::generateTriangle()
{
	Vertex1 v0;
	Vertex1 v1;
	Vertex1 v2;

	//Vertex data
	v0.Pos = DirectX::XMFLOAT3(0, 1, 0);
	v0.Color = DirectX::XMFLOAT4(0, 1, 0, 1); //green

	v1.Pos = DirectX::XMFLOAT3(1, 0, 0);
	v1.Color = DirectX::XMFLOAT4(1, 0, 0, 1); //red

	v2.Pos = DirectX::XMFLOAT3(-1, 0, 0);
	v2.Color = DirectX::XMFLOAT4(0, 0, 1, 1); //blue
	
	this->vertexData.push_back(v0);
	this->vertexData.push_back(v1);
	this->vertexData.push_back(v2);
}

void Model::initializeTriangle(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext, const DirectX::XMFLOAT3& pos)
{
	HRESULT result;
	//place triangle in world
	this->worldPos = pos;

	//raw vertex data
	this->generateTriangle();

	DirectX::XMFLOAT3X3 worldCoord;
	for (int i = 0; i < 3; i++)
	{
		Vertex1 temp;
		temp = this->vertexData.at(i);

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		x = temp.Pos.x;
		y = temp.Pos.y;
		z = temp.Pos.z;

		worldCoord.m[i][0] = x;
		worldCoord.m[i][1] = y;
		worldCoord.m[i][2] = z;
	}

	this->translationMatrix = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	this->rotateModelY(0);
	this->setUniformScale(1);

	//this->spinnY(0.001);
	this->uniformScaleIndication(0.0001);

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(Vertex1) * this->vertexData.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = false;
	desc.StructureByteStride = 0;

	//dynamic buffers does not need initial data
	result = gDevice->CreateBuffer(&desc, nullptr, &this->vertexBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	
	//map the memory, so that it cant be used in gpu while we change it
	result = gDeviceContext->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	//read the data and save them in a variable
	Vertex1* v = reinterpret_cast<Vertex1*>(mappedData.pData);
	UINT size = this->vertexData.size();

	//finally write the data into the vertex buffer
	for (UINT i = 0; i < size; i++)
	{
		v[i] = this->vertexData.at(i);
	}

	gDeviceContext->Unmap(this->vertexBuffer, 0);

}

ID3D11Buffer* Model::getVertexBuffer() const
{
	return this->vertexBuffer;
}

DirectX::XMMATRIX Model::getWorldModel() const
{
	return DirectX::XMMatrixTranspose(this->worldMatrix);
}

void Model::setUniformScale(const float & scalar)
{
	this->scaleMatrix = DirectX::XMMatrixScaling(scalar, scalar, scalar);
}

void Model::rotateModelY(const float & degree)
{
	this->rotationMatrix = DirectX::XMMatrixRotationY(degree);
}

void Model::spinnY(const float & degree)
{
	this->passiveSpinning = degree;
	this->isSpinning = true;
}

void Model::uniformScaleIndication(const float & speed)
{
	this->scaleSpeed = speed;
	this->tempScale = 1.0f;
	this->isScaling = true;
	this->scaleState = true;
}

void Model::update()
{
	if (this->isSpinning == true)
	{
		this->rotationMatrix *= DirectX::XMMatrixRotationY(this->passiveSpinning);
	}

	if (this->isScaling == true)
	{
		if (this->tempScale < 3.0000 && this->scaleState == true)
		{
			//grow state
			this->tempScale += this->scaleSpeed;
			this->setUniformScale(tempScale);
		}
		else if (this->tempScale > 1.0f && this->scaleState == false)
		{
			//shrink state
			this->tempScale -= this->scaleSpeed;
			this->setUniformScale(tempScale);
		}
		else
		{
			//swap state
			this->scaleState = !this->scaleState;
		}
	}
	this->worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;
}

void Model::shutdown()
{
	if (this->vertexBuffer != nullptr)
	{
		this->vertexBuffer->Release();
		this->vertexBuffer = nullptr;
	}
}


