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

	for (UINT ti = 0; ti < triangleCount; ti++)
	{
		/*VertexData v1 = VertexBuffer[IndexBuffer[i * 3]];
		VertexData v2 = VertexBuffer[IndexBuffer[i * 3 + 1]];
		VertexData v3 = VertexBuffer[IndexBuffer[i * 3 + 2]];
		VertexData temp;

		// 1. 삼각형 정점 소팅
		if (v1.Position.Y < v2.Position.Y)
		{
			temp = v1;
			v1 = v2;
			v2 = temp;
		}
		if (v1.Position.Y < v3.Position.Y)
		{
			temp = v1;
			v1 = v3;
			v3 = temp;
		}
		if (v2.Position.Y < v3.Position.Y)
		{
			temp = v2;
			v2 = v3;
			v3 = temp;
		}

		if (Math::Abs(v1.Position.Y - v2.Position.Y) > 0.00001f)
		{
			if (v2.Position.X > v3.Position.X)
			{
				temp = v2;
				v2 = v3;
				v3 = temp;
			}
			if (Math::Abs(v2.Position.Y - v3.Position.Y) > 0.00001f)
			{
			}
		}
		else
		{
			if (v1.Position.X > v2.Position.X)
			{
				temp = v1;
				v1 = v2;
				v2 = temp;
			}
		}*/

		VertexData tv[3] = {
			VertexBuffer[IndexBuffer[ti * 3]],
			VertexBuffer[IndexBuffer[ti * 3 + 1]],
			VertexBuffer[IndexBuffer[ti * 3 + 2]]
		};
		VertexData temp;

		// TO-DO. 퇴화 삼각형인지 식별.

		// 1. 삼각형 정점 소팅
		// 1-1. 0번과 1번의 Y값 비교
		if (tv[0].Position.Y == tv[1].Position.Y)
		{
			// X 값을 비교.
			if (tv[0].Position.X > tv[1].Position.X)
			{
				// 0번과 1번을 Swap
				temp = tv[0];
				tv[0] = tv[1];
				tv[1] = temp;
			}
		}
		else
		{
			if (tv[0].Position.Y < tv[1].Position.Y)
			{
				// 0번과 1번을 Swap
				temp = tv[0];
				tv[0] = tv[1];
				tv[1] = temp;
			}
		}

		if (tv[1].Position.Y == tv[2].Position.Y)
		{
			// X 값을 비교.
			if (tv[1].Position.X > tv[2].Position.X)
			{
				// 1번과 2번을 Swap
				temp = tv[1];
				tv[1] = tv[2];
				tv[2] = temp;
			}
		}
		else
		{
			if (tv[1].Position.Y < tv[2].Position.Y)
			{
				// 1번과 2번을 Swap
				temp = tv[1];
				tv[1] = tv[2];
				tv[2] = temp;
			}
		}


		if (tv[0].Position.Y == tv[1].Position.Y)
		{
			// X 값을 비교.
			if (tv[0].Position.X > tv[1].Position.X)
			{
				// 0번과 1번을 Swap
				temp = tv[0];
				tv[0] = tv[1];
				tv[1] = temp;
			}
		}
		else
		{
			if (tv[0].Position.Y < tv[1].Position.Y)
			{
				// 0번과 1번을 Swap
				temp = tv[0];
				tv[0] = tv[1];
				tv[1] = temp;
			}
		}



		// 2. 삼각형 패턴 파악 (Top-Flat, Bottom-Flat, Normal)

		if (tv[0].Position.Y == tv[1].Position.Y)
		{
			DrawTopFlatTriangle(tv);
		}
		else if (tv[1].Position.Y == tv[2].Position.Y)
		{
			DrawBottomFlatTriangle(tv);
		}
		else
		{
			// 삼각형을 두 개로 쪼갠다.
			VertexData newV = tv[1];
			float height = tv[0].Position.Y - tv[2].Position.Y;
			float width = tv[2].Position.X - tv[0].Position.X;

			if (height == 0.0f)
			{
				return;
			}

			float gradient = width / height;
			newV.Position.X = gradient * (tv[0].Position.Y - tv[1].Position.Y) + tv[0].Position.X;

			if (newV.Position.X > tv[1].Position.X)
			{
				VertexData upperTriangle[3] = { tv[0], tv[1], newV };
				VertexData bottomTriangle[3] = { tv[1], newV, tv[2] };
				DrawTopFlatTriangle(bottomTriangle);
				DrawBottomFlatTriangle(upperTriangle);
			}
			else
			{
				VertexData upperTriangle[3] = { tv[0], newV, tv[1] };
				VertexData bottomTriangle[3] = { newV, tv[1], tv[2] };
				DrawTopFlatTriangle(bottomTriangle);
				DrawBottomFlatTriangle(upperTriangle);
			}
		}

		// 3. Top-Flat, Bottom-Flat은 해당 로직으로,
		// Normal은 두 개로 분리해서 상하를 나눠서 그리기

		



		/*
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
		}*/
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

void WindowsRSI::DrawTopFlatTriangle(VertexData * tvs, bool DrawLastLine)
{
	float dx1 = tvs[0].Position.X - tvs[2].Position.X;
	float dx2 = tvs[1].Position.X - tvs[2].Position.X;
	float dy = tvs[2].Position.Y - tvs[1].Position.Y;


	if (dy >= 0)
	{
		return;
	}

	float a1 = dx1 / dy;
	float a2 = dx2 / dy;


	// Barycentric Coord
	Vector3 u = tvs[1].Position - tvs[0].Position;
	Vector3 v = tvs[2].Position - tvs[0].Position;
	float dotUU = u.Dot(u);
	float dotUV = u.Dot(v);
	float dotVV = v.Dot(v);
	float invDenom = 1.0f / (dotUU * dotVV - dotUV * dotUV);

	Vector3 w = tvs[2].Position - tvs[0].Position;
	float dotUW = u.Dot(w);
	float dotVW = v.Dot(w);
	float s = (dotVV * dotUW - dotUV * dotVW) * invDenom;
	float t = (dotUU * dotVW - dotUV * dotUW) * invDenom;

	float finalR = tvs[0].Color.R * (1 - s - t) + tvs[1].Color.R * s + tvs[2].Color.R * t;
	float finalG = tvs[0].Color.G * (1 - s - t) + tvs[1].Color.G * s + tvs[2].Color.G * t;
	float finalB = tvs[0].Color.B * (1 - s - t) + tvs[1].Color.B * s + tvs[2].Color.B * t;


	SetPixel(ScreenPoint(tvs[2].Position), LinearColor(1.f, 0.f, 0.f));
	float startX = tvs[2].Position.X;
	float startY = tvs[2].Position.Y;
	float currentY = floorf(tvs[2].Position.Y) - 0.5f;
	float destY = tvs[1].Position.Y;

	while (currentY <= destY)
	{
		float deltaY = startY - currentY;
		float leftX = a1 * deltaY + startX;
		float rightX = a2 * deltaY + startX;
		int pixelX1 = Math::FloorToInt(leftX);
		int pixelX2 = Math::FloorToInt(rightX);
		int pixelY = Math::FloorToInt(currentY);
		for (int p = pixelX1; p <= pixelX2; ++p)
		{
			Vector3 currentPosition = ScreenPoint(p, pixelY).ToVector3();

			// barycentric coord
			w = currentPosition - tvs[0].Position;
			dotUW = u.Dot(w);
			dotVW = v.Dot(w);
			s = (dotVV * dotUW - dotUV * dotVW) * invDenom;
			t = (dotUU * dotVW - dotUV * dotUW) * invDenom;

			finalR = tvs[0].Color.R * (1 - s - t) + tvs[1].Color.R * s + tvs[2].Color.R * t;
			finalG = tvs[0].Color.G * (1 - s - t) + tvs[1].Color.G * s + tvs[2].Color.G * t;
			finalB = tvs[0].Color.B * (1 - s - t) + tvs[1].Color.B * s + tvs[2].Color.B * t;

			SetPixel(ScreenPoint(p, pixelY), LinearColor(1.f, 0.f, 0.f));
		}
		currentY += 1.0f;
	}

	if (DrawLastLine)
	{
		// 마지막 라인을 그린다.
		int pixelX1 = Math::FloorToInt(tvs[1].Position.X);
		int pixelX2 = Math::FloorToInt(tvs[2].Position.X);
		int pixelY = Math::FloorToInt(destY);
		for (int p = pixelX1; p <= pixelX2; ++p)
		{
			SetPixel(ScreenPoint(p, pixelY), LinearColor(1.f, 0.f, 0.f));
		}
	}
}

void WindowsRSI::DrawBottomFlatTriangle(VertexData * tvs)
{
	float dx1 = tvs[1].Position.X - tvs[0].Position.X;
	float dx2 = tvs[2].Position.X - tvs[0].Position.X;
	float dy = tvs[0].Position.Y - tvs[1].Position.Y;

	if (dy <= 0)
	{
		return;
	}

	float gradient1 = dx1 / dy;
	float gradient2 = dx2 / dy;

	SetPixel(ScreenPoint(tvs[0].Position), LinearColor(0.f, 1.f, 0.f));
	float startY = tvs[0].Position.Y;
	float startX = tvs[0].Position.X;
	float currentY = floorf(tvs[0].Position.Y) - 0.5f;
	float destY = tvs[1].Position.Y;
	while (currentY >= destY)
	{
		float deltaY = startY - currentY;
		float leftX = gradient1 * deltaY + startX;
		float rightX = gradient2 * deltaY + startX;
		int startX = Math::FloorToInt(leftX);
		int endX = Math::FloorToInt(rightX);
		int pixelY = Math::FloorToInt(currentY);
		for (int p = startX; p <= endX; ++p)
		{
			SetPixel(ScreenPoint(p, pixelY), LinearColor(0.f, 1.f, 0.f));
		}
		currentY -= 1.0f;
	}

	// 마지막 라인을 그린다.
	int pixelX1 = Math::FloorToInt(tvs[0].Position.X);
	int pixelX2 = Math::FloorToInt(tvs[1].Position.X);
	int pixelY = Math::FloorToInt(destY);
	for (int p = pixelX1; p <= pixelX2; ++p)
	{
		SetPixel(ScreenPoint(p, pixelY), LinearColor(0.f, 1.f, 0.f));
	}
}
