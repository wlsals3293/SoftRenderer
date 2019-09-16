#include "Precompiled.h"
#include "..\Public\TriangleRasterizer.h"

TriangleRasterizer::TriangleRasterizer(VertexData InV1, VertexData InV2, VertexData InV3)
{
	VertexBuffer[0] = InV1;
	VertexBuffer[1] = InV2;
	VertexBuffer[2] = InV3;

	RecalBounds();
}

void TriangleRasterizer::RecalBounds()
{
	Vector2 tempVec;

	tempVec.X = Math::Min(VertexBuffer[0].Position.X, VertexBuffer[1].Position.X);
	tempVec.X = Math::Min(tempVec.X, VertexBuffer[2].Position.X);
	tempVec.Y = Math::Min(VertexBuffer[0].Position.Y, VertexBuffer[1].Position.Y);
	tempVec.Y = Math::Min(tempVec.Y, VertexBuffer[2].Position.Y);
	TopLeft = ScreenPoint(Math::TruncToInt(tempVec.X), Math::TruncToInt(tempVec.Y));

	tempVec.X = Math::Max(VertexBuffer[0].Position.X, VertexBuffer[1].Position.X);
	tempVec.X = Math::Max(tempVec.X, VertexBuffer[2].Position.X);
	tempVec.Y = Math::Max(VertexBuffer[0].Position.Y, VertexBuffer[1].Position.Y);
	tempVec.Y = Math::Max(tempVec.Y, VertexBuffer[2].Position.Y);
	BottomRight = ScreenPoint(Math::TruncToInt(tempVec.X), Math::TruncToInt(tempVec.Y));

	UVector = VertexBuffer[1].Position - VertexBuffer[0].Position;
	VVector = VertexBuffer[2].Position - VertexBuffer[0].Position;
	DotUU = UVector.Dot(UVector);
	DotUV = UVector.Dot(VVector);
	DotUV = VVector.Dot(VVector);
	InvDenom = 1.0f / (DotUU * DotVV - DotUV * DotUV);
}
