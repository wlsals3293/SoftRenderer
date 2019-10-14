
#include "WindowsPrecompiled.h"
#include "SoftRenderer.h"
#include "WindowsRSI.h"

void SoftRenderer::Initialize(HWND InWnd)
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	MilliSecFrequency = frequency.QuadPart / 1000.0f;

	FrameCount = 0;

	RSI = new WindowsRSI();
	if (RSI != nullptr)
	{
		RSI->Init(false);
	}
}

void SoftRenderer::Shutdown()
{
	if (RSI != nullptr && RSI->IsIntialized())
	{
		RSI->Shutdown();
		delete RSI;
		RSI = nullptr;
	}
}

void SoftRenderer::PreUpdate()
{
	LARGE_INTEGER cycles;
	QueryPerformanceCounter(&cycles);
	FrameTime = cycles.QuadPart;
	if (FrameCount == 0)
	{
		StartFrameTime = cycles.QuadPart;
	}
}

void SoftRenderer::Update()
{
	if (RSI != nullptr)
	{
		RSI->Clear(LinearColor(0.125f, 0.5f, 1.f, 1.f));

		/*RSI->SetColor(LinearColor(1.f, 0.f, 0.f, 1.f));

		VertexData vertices[6];
		vertices[0].Position = Vector2(260.f, 100.f);
		vertices[1].Position = Vector2(200.f, 200.f);
		vertices[2].Position = Vector2(130.f, 70.f);
		
		vertices[3].Position = Vector2(-230.f, -200.f);
		vertices[4].Position = Vector2(-50.f, -250.f);
		vertices[5].Position = Vector2(-100.f, -50.f);

		int indices[6];
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 3;
		indices[4] = 4;
		indices[5] = 5;

		// Draw Call
		RSI->SetVertexBuffer(vertices);
		RSI->SetIndexBuffer(indices);
		RSI->DrawPrimitive(6, 6);*/

		// Draw Grid
		ScreenPoint screenSize = DisplaySetting::Inst().GetSize();
		ScreenPoint halfSize = screenSize.GetHalfSize();
		LinearColor gridColor(0.6f, 0.6f, 0.6f, 1.f);
		int padding = 10;
		int verticalCount = Math::TruncToInt(halfSize.X / padding);
		int horizontalCount = Math::TruncToInt(halfSize.Y / padding);

		for (int i = 1; i < verticalCount; i++)
		{
			RSI->DrawVerticalLine(i * padding, gridColor);
			RSI->DrawVerticalLine(i * -padding, gridColor);
		}

		for (int i = 1; i < horizontalCount; i++)
		{
			RSI->DrawHorizontalLine(i * padding, gridColor);
			RSI->DrawHorizontalLine(i * -padding, gridColor);
		}

		RSI->DrawVerticalLine(0, LinearColor::Green);
		RSI->DrawHorizontalLine(0, LinearColor::Red);

		/*
		// 1사분면
		RSI->DrawLine(Vector2(30.f, 30.f), Vector2(100.f, 200.f), LinearColor::White);
		RSI->DrawLine(Vector2(30.f, 30.f), Vector2(200.f, 100.f), LinearColor::White);

		// 2사분면
		RSI->DrawLine(Vector2(-30.f, 30.f), Vector2(-100.f, 200.f), LinearColor::White);
		RSI->DrawLine(Vector2(-30.f, 30.f), Vector2(-200.f, 100.f), LinearColor::White);

		// 3사분면
		RSI->DrawLine(Vector2(-30.f, -30.f), Vector2(-100.f, -200.f), LinearColor::White);
		RSI->DrawLine(Vector2(-30.f, -30.f), Vector2(-200.f, -100.f), LinearColor::White);

		// 4사분면
		RSI->DrawLine(Vector2(30.f, -30.f), Vector2(100.f, -200.f), LinearColor::White);
		RSI->DrawLine(Vector2(30.f, -30.f), Vector2(200.f, -100.f), LinearColor::White);
		*/


		Matrix2x2 resultMat, rotateMat, scaleMat;

		static float angleSpeedPerSec = 180.f;
		float currentAngle = ElapsedTime * angleSpeedPerSec;
		rotateMat.SetRotation(Math::Deg2Rad(currentAngle));

		static Vector2 minScale(0.8f, 0.8f);
		static Vector2 maxScale(1.0f, 1.0f);
		float sin, cos;
		Math::GetSinCos(sin, cos, currentAngle * 2);

		Vector2 calVector = (maxScale - minScale) * ((sin+1.f) / 2.f);

		scaleMat.SetScale(minScale + calVector);

		resultMat = rotateMat * scaleMat;

		// Define Mesh Data
		VertexData v[9];
		v[0].Position = Vector2(-150.0f, -100.0f);
		v[0].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[1].Position = Vector2(-200.0f, 100.0f);
		v[1].Color = LinearColor(0.0f, 1.0f, 0.0f);
		v[2].Position = Vector2(-100.0f, 100.0f);
		v[2].Color = LinearColor(0.0f, 0.0f, 1.0f);

		v[3].Position = Vector2(150.0f, 100.0f);
		v[3].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[4].Position = Vector2(100.0f, -100.0f);
		v[4].Color = LinearColor(0.0f, 1.0f, 0.0f);
		v[5].Position = Vector2(200.0f, -100.0f);
		v[5].Color = LinearColor(0.0f, 0.0f, 1.0f);

		v[6].Position = Vector2(50.0f, 100.0f);
		v[6].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[7].Position = Vector2(-50.0f, 0.0f);
		v[7].Color = LinearColor(0.0f, 1.0f, 0.0f);
		v[8].Position = Vector2(0.0f, -100.0f);
		v[8].Color = LinearColor(0.0f, 0.0f, 1.0f);

		int i[9];
		i[0] = 0;
		i[1] = 1;
		i[2] = 2;
		i[3] = 3;
		i[4] = 4;
		i[5] = 5;
		i[6] = 6;
		i[7] = 7;
		i[8] = 8;


		for (int i = 0; i < 9; i++)
		{
			v[i].Position *= resultMat;
		}

		// Draw Call
		RSI->SetVertexBuffer(v);
		RSI->SetIndexBuffer(i);
		RSI->DrawPrimitive(9, 9);


		RSI->EndFrame();
	}
}

void SoftRenderer::PostUpdate()
{
	FrameCount++;
	LARGE_INTEGER currentFrameCycles;
	QueryPerformanceCounter(&currentFrameCycles);
	FrameMilliSec = double(currentFrameCycles.QuadPart - FrameTime) / MilliSecFrequency;
	FrameSec = (float)(FrameMilliSec / 1000);
	ElapsedTime += FrameSec;
	FrameFPS = FrameSec == 0.f ? 0.f : 1.f / FrameSec;

}


