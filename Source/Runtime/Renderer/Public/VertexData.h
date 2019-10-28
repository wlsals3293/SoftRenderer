#pragma once

#include "DisplaySetting.h"
#include "LinearColor.h"
#include "Vector2.h"
#include "Vector3.h"

struct VertexData
{
	VertexData(Vector3 InV) : Position(InV) {}

	Vector3 Position;
	LinearColor Color;
	Vector2 UV;
};


