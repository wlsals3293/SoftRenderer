
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

		/*
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
		quad.GetTransform().AddRotation(angleSpeedPerSec * FrameSec);*/


		static Vector3 cubePos = Vector3::Zero;
		static Vector3 cubeRotation = Vector3::Zero;

		float moveSpeed = 200.f;
		float rotateSpeed = 180.f;
		float currentScale = 100.f;

		float cr, sr, cp, sp, cy, sy;

		Math::GetSinCos(sr, cr, cubeRotation.Z);
		Math::GetSinCos(sp, cp, cubeRotation.X);
		Math::GetSinCos(sy, cy, cubeRotation.Y);


		Matrix4x4 rMat = Matrix4x4(
			Vector4(cr*cy + sr * sp*sy, sr*cy - cr * sp*sy, cp*-sy, 0.f),
			Vector4(-sr * cp, cr*cp, -sp, 0.f),
			Vector4(cr*sy - sr * sp*cy, sr*sy + cr * sp*cy, 0),
			Vector4(0.f, 0.f, 0.f, 1.f)
		);

		//Matrix4x4 sMat = ;

		Matrix4x4 fMat = rMat;

		float FOV = 90.f;

		float a = (float)screenSize.X / (float)screenSize.Y;
		float repA = (float)screenSize.Y / (float)screenSize.X;
		float d = 1.f / tanf(FOV * 0.5f);

		// Define Mesh Data
		const int vertexCount = 24;
		VertexData v[vertexCount] = {
			// Front
			VertexData(Vector3(0.5f, -0.5f, 0.5f)),
			VertexData(Vector3(0.5f, 0.5f, 0.5f)),
			VertexData(Vector3(0.5f, 0.5f, -0.5f)),
			VertexData(Vector3(0.5f, -0.5f, -0.5f)),
			// Left
			VertexData(Vector3(-0.5f, -0.5f, 0.5f)),
			VertexData(Vector3(-0.5f, 0.5f, 0.5f)),
			VertexData(Vector3(0.5f, 0.5f, 0.5f)),
			VertexData(Vector3(0.5f, -0.5f, 0.5f)),
			// Right
			VertexData(Vector3(0.5f, -0.5f, -0.5f)),
			VertexData(Vector3(0.5f, 0.5f, -0.5f)),
			VertexData(Vector3(-0.5f, 0.5f, -0.5f)),
			VertexData(Vector3(-0.5f, -0.5f, -0.5f)),
			// Back
			VertexData(Vector3(-0.5f, -0.5f, -0.5f)),
			VertexData(Vector3(-0.5f, 0.5f, -0.5f)),
			VertexData(Vector3(-0.5f, 0.5f, 0.5f)),
			VertexData(Vector3(-0.5f, -0.5f, 0.5f)),
			// Top
			VertexData(Vector3(0.5f, 0.5f, 0.5f)),
			VertexData(Vector3(-0.5f, 0.5f, 0.5f)),
			VertexData(Vector3(-0.5f, 0.5f, -0.5f)),
			VertexData(Vector3(0.5f, 0.5f, -0.5f)),
			// Bottom
			VertexData(Vector3(-0.5f, -0.5f, 0.5f)),
			VertexData(Vector3(0.5f, -0.5f, 0.5f)),
			VertexData(Vector3(0.5f, -0.5f, -0.5f)),
			VertexData(Vector3(-0.5f, -0.5f, -0.5f))
		};

		const int triangleCount = 12;
		const int indexCount = triangleCount * 3;
		int i[indexCount] = {
		 0, 2, 1, 0, 3, 2,
		 4, 6, 5, 4, 7, 6,
		 8, 10, 9, 8, 11, 10,
		 12, 14, 13, 12, 15, 14,
		 16, 18, 17, 16, 19, 18,
		 20, 22, 21, 20, 23, 22
		};

		//for (int i = 0; i < vertexCount; i++)
		//{
			//v[i].Position *= fMat;
		//}

		for (int t = 0; t < triangleCount; t++)
		{
			Vector4 tp[3];
			tp[0] = v[i[t * 3]].Position;
			tp[1] = v[i[t * 3 + 1]].Position;
			tp[2] = v[i[t * 3 + 2]].Position;

			for (int ti = 0; ti < 3; ti++)
			{
				tp[ti] = fMat * tp[ti];

				float repZ = 1.f / -tp[ti].Z;
				tp[ti].Y = tp[ti].Y * d * repZ;
				tp[ti].X = tp[ti].X * d * repZ * repA;

				tp[ti].X *= (screenSize.X * 0.5f);
				tp[ti].Y *= (screenSize.Y * 0.5f);
			}
			RSI->DrawLine(tp[0], tp[1], LinearColor::Red);
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


