#include "Precompiled.h"
#include "..\Public\TriangleRasterizer.h"

TriangleRasterizer::TriangleRasterizer(VertexData InV1, VertexData InV2, VertexData InV3)
{
	VertexBuffer[0] = InV1;
	VertexBuffer[1] = InV2;
	VertexBuffer[2] = InV3;

	RecalcBounds();
}

void TriangleRasterizer::RecalcBounds()
{
	Vector2 sbbMin2D = Vector2(INFINITY, INFINITY);
	Vector2 sbbMax2D = Vector2(-INFINITY, -INFINITY);

	for (int i = 0; i < 3; i++)
	{
		if (VertexBuffer[i].Position.X < sbbMin2D.X) sbbMin2D.X = VertexBuffer[i].Position.X;
		if (VertexBuffer[i].Position.Y < sbbMin2D.Y) sbbMin2D.Y = VertexBuffer[i].Position.Y;
		if (VertexBuffer[i].Position.X > sbbMax2D.X) sbbMax2D.X = VertexBuffer[i].Position.X;
		if (VertexBuffer[i].Position.Y > sbbMax2D.Y) sbbMax2D.Y = VertexBuffer[i].Position.Y;
	}

	//BottomRight = ScreenPoint(sbbMax2D.X, sbbMin2D.Y);
	//TopLeft = ScreenPoint(sbbMin2D.X, sbbMax2D.Y);
	/*
	tempVec.X = Math::Min(VertexBuffer[0].Position.X, VertexBuffer[1].Position.X);
	tempVec.X = Math::Min(tempVec.X, VertexBuffer[2].Position.X);
	tempVec.Y = Math::Max(VertexBuffer[0].Position.Y, VertexBuffer[1].Position.Y);
	tempVec.Y = Math::Max(tempVec.Y, VertexBuffer[2].Position.Y);
	TopLeft = ScreenPoint(Math::TruncToInt(tempVec.X), Math::TruncToInt(tempVec.Y));

	tempVec.X = Math::Max(VertexBuffer[0].Position.X, VertexBuffer[1].Position.X);
	tempVec.X = Math::Max(tempVec.X, VertexBuffer[2].Position.X);
	tempVec.Y = Math::Min(VertexBuffer[0].Position.Y, VertexBuffer[1].Position.Y);
	tempVec.Y = Math::Min(tempVec.Y, VertexBuffer[2].Position.Y);
	BottomRight = ScreenPoint(Math::TruncToInt(tempVec.X), Math::TruncToInt(tempVec.Y));
	*/
	UVector = VertexBuffer[1].Position - VertexBuffer[0].Position;
	VVector = VertexBuffer[2].Position - VertexBuffer[0].Position;
	DotUU = UVector.Dot(UVector);
	DotUV = UVector.Dot(VVector);
	DotVV = VVector.Dot(VVector);
	InvDenom = 1.0f / (DotUU * DotVV - DotUV * DotUV);
}
