
#include "Precompiled.h"
#include "WindowsRSI.h"
#include "TriangleRasterizer.h"

void WindowsRSI::Init(const bool InbSRGBColorSpace)
{
	InitializeGDI(DisplaySetting::Inst().GetSize(), InbSRGBColorSpace);
}

void WindowsRSI::Shutdown()
{
	ReleaseGDI();
}

void WindowsRSI::Clear(const LinearColor & InClearColor)
{
	SetColor(InClearColor);
	FillBuffer();

	ClearDepthBuffer();
}

void WindowsRSI::BeginFrame()
{

}

void WindowsRSI::SetVertexBuffer(VertexData * InVertexData)
{
	VertexBuffer = InVertexData;
}

void WindowsRSI::SetIndexBuffer(const int * InIndexData)
{
	IndexBuffer = InIndexData;
}

void WindowsRSI::DrawPrimitive(UINT InVertexSize, UINT InIndexSize)
{
	if (VertexBuffer == nullptr || IndexBuffer == nullptr)
	{
		return;
	}

	UINT triangleCount = (int)(InIndexSize / 3);

	for (UINT i = 0; i < triangleCount; i++)
	{
		TriangleRasterizer tr(VertexBuffer[i * 3],
			VertexBuffer[i * 3 + 1],
			VertexBuffer[i * 3 + 2]);

		for (int x = tr.TopLeft.X; x < tr.BottomRight.X; x++)
		{
			for (int y = tr.TopLeft.Y; y < tr.BottomRight.Y; y++)
			{
				ScreenPoint currentPixel(x, y);
				Vector2 currentPos = currentPixel.ToVector2();
				if (tr.IsInside(currentPos))
				{
					PutPixel(currentPixel, LinearColor(1.f, 0.f, 0.f));
				}
			}
		}
	}
}

void WindowsRSI::DrawTriangle(const Vector2& P1, const Vector2& P2, const Vector2& P3)
{
	Vector2 minPos = Vector2::Zero;
	Vector2 maxPos = Vector2::Zero;

	minPos.X = Math::Min(P1.X, P2.X);
	minPos.X = Math::Min(minPos.X, P3.X);
	minPos.Y = Math::Min(P1.Y, P2.Y);
	minPos.Y = Math::Min(minPos.Y, P3.Y);

	maxPos.X = Math::Max(P1.X, P2.X);
	maxPos.X = Math::Max(maxPos.X, P3.X);
	maxPos.Y = Math::Max(P1.Y, P2.Y);
	maxPos.Y = Math::Max(maxPos.Y, P3.Y);

	Vector2 u = P2 - P1;
	Vector2 v = P3 - P1;
	float dotUU = u.Dot(u);
	float dotUV = u.Dot(v);
	float dotVV = v.Dot(v);
	float invDenom = 1.0f / (dotUU * dotVV - dotUV * dotUV);

	ScreenPoint minPoint(Math::TruncToInt(minPos.X), Math::TruncToInt(minPos.Y));
	ScreenPoint maxPoint(Math::TruncToInt(maxPos.X), Math::TruncToInt(maxPos.Y));

	for (int x = minPoint.X; x < maxPoint.X; x++)
	{
		for (int y = minPoint.Y; y < maxPoint.Y; y++)
		{
			ScreenPoint point(x, y);
			Vector2 w = Vector2(point.X, point.Y) - P1;
			float dotUW = u.Dot(w);
			float dotVW = v.Dot(w);
			float s = (dotVV * dotUW - dotUV * dotVW) * invDenom;
			float t = (dotUU * dotVW - dotUV * dotUW) * invDenom;
			if (s >= 0 && t >= 0 && ((s + t) <= 1))
			{
				PutPixel(point);
			}
		}
	}
}
