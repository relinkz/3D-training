#pragma once
#include "GlobalData.h"

class Camera
{
public:
	Camera();
	virtual ~Camera();

	DirectX::XMMATRIX getViewMatrix() const;
	DirectX::XMMATRIX getProjectionMatrix() const;
	DirectX::XMMATRIX getOrthogMatrix() const;

	void rotateCameraY(float radius);
private:
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR target;
	DirectX::XMVECTOR upVector;

	float perspAngle;
	float nearPlane;
	float farPlane;
};

