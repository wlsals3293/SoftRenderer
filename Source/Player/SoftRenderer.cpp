
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

		// Draw Grid
		ScreenPoint screenSize = DisplaySetting::Inst().GetSize();
		ScreenPoint halfSize = screenSize.GetHalfSize();
		LinearColor gridColor(0.6f, 0.6f, 0.6f, 1.f);
		int padding = 10;
		int verticalCount = Math::TruncToInt(halfSize.X / (float)padding);
		int horizontalCount = Math::TruncToInt(halfSize.Y / (float)padding);

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


		Matrix3x3 resultMat, translationMat, rotateMat, scaleMat;

		translationMat.SetTranslation(250.f, -180.f);
		/*
		static float angleSpeedPerSec = 180.f;
		float currentAngle = ElapsedTime * angleSpeedPerSec;
		rotateMat.SetRotation(Math::Deg2Rad(currentAngle));

		static Vector2 minScale(0.8f, 0.8f);
		static Vector2 maxScale(1.0f, 1.0f);
		float sin, cos;
		Math::GetSinCos(sin, cos, currentAngle * 2);

		Vector2 calVector = (maxScale - minScale) * ((sin+1.f) / 2.f);

		scaleMat.SetScale(minScale + calVector);*/
		
		resultMat = translationMat;


		// Define Mesh Data
		VertexData v[9];
		v[0].Position = Vector3(-60.0f, -40.0f, 1.f);
		v[0].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[1].Position = Vector3(30.0f, 0.0f, 1.f);
		v[1].Color = LinearColor(0.0f, 1.0f, 0.0f);
		v[2].Position = Vector3(0.0f, 100.0f, 1.f);
		v[2].Color = LinearColor(0.0f, 0.0f, 1.0f);

		int i[3];
		i[0] = 0;
		i[1] = 1;
		i[2] = 2;


		for (int i = 0; i < 3; i++)
		{
			v[i].Position *= resultMat;
		}

		// Draw Call
		RSI->SetVertexBuffer(v);
		RSI->SetIndexBuffer(i);
		RSI->DrawPrimitive(3, 3);


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


