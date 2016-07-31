#include "Camera.h"

Camera::Camera()
{
	this->position	= DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)));
	this->target	= DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)));
	this->upVector	= DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)));
}

Camera::~Camera()
{
}

DirectX::XMMATRIX Camera::getViewMatrix() const
{
	DirectX::XMMATRIX toReturn;
	DirectX::XMFLOAT4X4 values;
	toReturn = DirectX::XMMatrixLookAtLH(this->position, this->target, this->upVector);

	return toReturn;
}

DirectX::XMMATRIX Camera::getProjectionMatrix() const
{
	DirectX::XMMATRIX toReturn;
	return toReturn;
}

DirectX::XMMATRIX Camera::getOrthogMatrix() const
{
	DirectX::XMMATRIX toReturn;
	return toReturn;
}
