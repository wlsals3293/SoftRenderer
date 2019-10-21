#pragma once

#include "Transform2D.h"


class Camera2D
{
public:
	Camera2D() {}

	Matrix3x3 GetViewMatrix();

public:
	Transform2D Transform;
};