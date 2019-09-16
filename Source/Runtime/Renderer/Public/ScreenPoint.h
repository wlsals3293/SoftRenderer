#pragma once

#include "CoreDefinition.h"
#include "Vector2.h"

struct ScreenPoint
{
public:
	ScreenPoint() : X(0), Y(0) { }
	ScreenPoint(int InX, int InY) : X(InX), Y(InY) { }

	FORCEINLINE ScreenPoint GetHalfSize() const
	{
		return ScreenPoint((int)(X * 0.5f), (int)(Y * 0.5f));
	}

	FORCEINLINE Vector2 ToVector2() const
	{
		return Vector2((float)X, (float)Y);
	}

	int X;
	int Y;
};
