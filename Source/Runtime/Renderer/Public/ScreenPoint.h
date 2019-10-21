#pragma once

#include "CoreDefinition.h"
#include "MathHeaders.h"

struct ScreenPoint
{
public:
	ScreenPoint() : X(0), Y(0) { }
	ScreenPoint(int InX, int InY) : X(InX), Y(InY) {}
	ScreenPoint(const Vector2& InVector) : X(Math::TruncToInt(InVector.X)), Y(Math::TruncToInt(InVector.Y)) {}
	ScreenPoint(const Vector3& InVector) : X(Math::TruncToInt(InVector.X)), Y(Math::TruncToInt(InVector.Y)) {}

	FORCEINLINE ScreenPoint GetHalfSize() const
	{
		return ScreenPoint((int)(X * 0.5f), (int)(Y * 0.5f));
	}

	FORCEINLINE Vector2 ToVector2() const
	{
		return Vector2((float)X, (float)Y);
	}

	FORCEINLINE Vector3 ToVector3() const
	{
		return Vector3((float)X, (float)Y);
	}

	int X;
	int Y;
};
