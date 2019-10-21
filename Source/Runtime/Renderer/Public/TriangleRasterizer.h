#pragma once

#include "Vector2.h"
#include "VertexData.h"


class TriangleRasterizer
{
public:
	TriangleRasterizer(VertexData InV1, VertexData InV2, VertexData InV3);

public:
	void RecalcBounds();

	FORCEINLINE bool IsInside(const Vector2& InVec) const
	{
		Vector2 st = GetBaryCentricCoord(InVec);
		if (st.X < 0.f) return false;
		if (st.Y < 0.f) return false;
		if (st.X + st.Y > 1.f) return false;

		return true;
	}

	FORCEINLINE Vector2 GetBaryCentricCoord(const Vector2& InScreenPosition) const
	{
		Vector2 w = InScreenPosition - Vector2(VertexBuffer[0].Position.X, VertexBuffer[0].Position.Y);
		float dotUW = UVector.Dot(w);
		float dotVW = VVector.Dot(w);
		float s = (DotVV * dotUW - DotUV * dotVW) * InvDenom;
		float t = (DotUU * dotVW - DotUV * dotUW) * InvDenom;
		return Vector2(s, t);
	}

	FORCEINLINE Vector2 GetUV(const Vector2& InScreenPosition) const
	{
		Vector2 st = GetBaryCentricCoord(InScreenPosition);
		float oneMinusST = 1 - st.X - st.Y;
		return VertexBuffer[0].UV * oneMinusST + VertexBuffer[1].UV * st.X + VertexBuffer[2].UV * st.Y;
	}


public:
	VertexData VertexBuffer[3];
	ScreenPoint TopLeft;
	ScreenPoint BottomRight;

private:
	Vector2 UVector;
	Vector2 VVector;
	float DotUU, DotUV, DotVV, InvDenom;
};