
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
	if (RSI == nullptr)
	{
		return;
	}

	RSI->Init(false);
	MainTexture.LoadPNGFile("Steve.png");
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


		

		float speed = 200.f;
		float currentScale = 100.f;
		static GameObject2D quad;
		static Camera2D camera;

		Vector2 deltaMove = Vector2(
			speed * InputManager.GetXAxis(),
			speed * InputManager.GetYAxis()
		) * FrameSec;

		camera.Transform.AddPosition(deltaMove);
		//quad.GetTransform().SetPosition(Vector2(150.f, 10.f));
		
		static float angleSpeedPerSec = 180.f;
		float currentAngle = ElapsedTime * angleSpeedPerSec;
		quad.GetTransform().AddRotation(angleSpeedPerSec * FrameSec);

		/*static Vector2 minScale(0.8f, 0.8f);
		static Vector2 maxScale(1.0f, 1.0f);
		float sin, cos;
		Math::GetSinCos(sin, cos, currentAngle);

		Vector2 calVector = (maxScale - minScale) * ((sin+1.f) * 0.5f);
		quad.GetTransform().SetScale(minScale + calVector);*/
		quad.GetTransform().SetScale(Vector2::One * currentScale);
		
		Matrix3x3 fMat = camera.GetViewMatrix() * quad.GetTransform().GetTRS();


		// Define Mesh Data
		const int vertexCount = 4;
		VertexData v[vertexCount];
		v[0].Position = Vector3(-0.5f, -0.5f, 1.f);
		v[0].Color = LinearColor(1.0f, 0.0f, 0.0f);
		v[0].UV = Vector2(0.f, 0.f);

		v[1].Position = Vector3(-0.5f, 0.5f, 1.f);
		v[1].Color = LinearColor(0.0f, 1.0f, 0.0f);
		v[1].UV = Vector2(0.f, 1.f);

		v[2].Position = Vector3(0.5f, 0.5f, 1.f);
		v[2].Color = LinearColor(0.0f, 0.0f, 1.0f);
		v[2].UV = Vector2(1.f, 1.f);

		v[3].Position = Vector3(0.5f, -0.5f, 1.f);
		v[3].Color = LinearColor(0.0f, 0.0f, 1.0f);
		v[3].UV = Vector2(1.f, 0.f);

		const int indexCount = 6;
		int i[indexCount] = { 0, 1, 2, 0, 2, 3 };

		for (int i = 0; i < vertexCount; i++)
		{
			v[i].Position *= fMat;
		}

		// Draw Call
		RSI->SetTexture(RSITexture(MainTexture.GetBuffer(), MainTexture.GetWidth(), MainTexture.GetHeight()));
		RSI->SetVertexBuffer(v);
		RSI->SetIndexBuffer(i);
		RSI->DrawPrimitive(vertexCount, indexCount);


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


