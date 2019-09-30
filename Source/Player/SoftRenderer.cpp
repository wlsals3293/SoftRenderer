
#include "WindowsPrecompiled.h"
#include "SoftRenderer.h"
#include "WindowsRSI.h"

void SoftRenderer::Initialize(HWND InWnd)
{
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

void SoftRenderer::Update()
{
	if (RSI != nullptr)
	{
		RSI->Clear(LinearColor(0.125f, 0.5f, 1.f, 1.f));

		/*
		Vector2 P1(70.f, 150.f);
		Vector2 P2(-150.f, 0.f);
		Vector2 P3(150.f, 0.f);

		RSI->SetColor(LinearColor(1.f, 0.f, 0.f, 1.f));
		RSI->DrawTriangle(P1, P2, P3);
		*/
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
		int padding = 100;
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


		RSI->DrawLine(Vector2(30.f, 30.f), Vector2(100.f, 200.f), LinearColor::White);


		RSI->EndFrame();
	}
}


