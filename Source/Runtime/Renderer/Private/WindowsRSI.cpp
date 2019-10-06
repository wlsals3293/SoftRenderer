
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

void WindowsRSI::Clear(const LinearColor& InClearColor)
{
	SetColor(InClearColor);
	FillBuffer();

	ClearDepthBuffer();
}

void WindowsRSI::BeginFrame()
{

}

void WindowsRSI::SetVertexBuffer(VertexData* InVertexData)
{
	VertexBuffer = InVertexData;
}

void WindowsRSI::SetIndexBuffer(const int* InIndexData)
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
			for (int y = tr.BottomRight.Y; y < tr.TopLeft.Y; y++)
			{
				ScreenPoint currentPixel(x, y);
				Vector2 currentPos = currentPixel.ToVector2();
				if (tr.IsInside(currentPos))
				{
					SetPixel(currentPixel, LinearColor(1.f, 0.f, 0.f));
				}
			}
		}
	}
}

void WindowsRSI::DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const LinearColor& InColor)
{
	ScreenPoint currentPos, endPos;

	int w = Math::Abs(InEndPos.X - InStartPos.X);
	int h = Math::Abs(InEndPos.Y - InStartPos.Y);
	int delta;

	// 가로 길이가 세로 길이보다 클 때
	if (w > h)
	{
		// 판별식
		int f = 2 * h - w;
		int dF1 = 2 * h;
		int dF2 = 2 * (h - w);

		// x 값이 작은 지점부터 시작
		currentPos = (InStartPos.X < InEndPos.X) ? InStartPos : InEndPos;
		endPos = (InStartPos.X < InEndPos.X) ? InEndPos : InStartPos;

		// 목표 지점보다 y가 작으면 + 아니면 -
		delta = (currentPos.Y < endPos.Y) ? 1 : -1;

		for (; currentPos.X <= endPos.X; currentPos.X++)
		{
			SetPixel(currentPos, InColor);

			if (f < 0)
			{
				f += dF1;
			}
			else
			{
				currentPos.Y += delta;
				f += dF2;
			}
		}
	}
	// 가로 길이가 세로 길이와 같거나 작을 때
	else
	{
		// 판별식
		int f = h - 2 * w;
		int dF1 = -2 * w;
		int dF2 = 2 * (h - w);

		// y 값이 작은 지점부터 시작
		currentPos = (InStartPos.Y < InEndPos.Y) ? InStartPos : InEndPos;
		endPos = (InStartPos.Y < InEndPos.Y) ? InEndPos : InStartPos;

		// 목표 지점보다 x가 작으면 + 아니면 -
		delta = (currentPos.X < endPos.X) ? 1 : -1;

		for (; currentPos.Y <= endPos.Y; currentPos.Y++)
		{
			SetPixel(currentPos, InColor);

			if (f > 0)
			{
				f += dF1;
			}
			else
			{
				currentPos.X += delta;
				f += dF2;
			}
		}
	}
}

void WindowsRSI::DrawVerticalLine(int InX, const LinearColor& InColor)
{
	Color32* dest = ScreenBuffer;
	unsigned long sizeX = ScreenSize.X;
	int pixelX = InX + Math::TruncToInt((float)ScreenSize.X * 0.5f);

	for (int i = 0; i < ScreenSize.Y; i++)
	{
		*(dest + pixelX + (sizeX * i)) = InColor.ToColor32();
	}
}

void WindowsRSI::DrawHorizontalLine(int InY, const LinearColor& InColor)
{
	Color32* dest = ScreenBuffer;
	int pixelY = Math::TruncToInt((float)ScreenSize.Y * 0.5f) - InY;

	dest += pixelY * ScreenSize.X;

	for (int i = 0; i < ScreenSize.X; i++)
	{
		*dest++ = InColor.ToColor32();
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
				SetPixel(point);
			}
		}
	}
}
