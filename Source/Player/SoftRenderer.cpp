
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

		Vector2 P1(70.f, 150.f);
		Vector2 P2(-150.f, 0.f);
		Vector2 P3(150.f, 0.f);

		RSI->SetColor(LinearColor(1.f, 0.f, 0.f, 1.f));
		RSI->DrawTriangle(P1, P2, P3);

		RSI->EndFrame();
	}
}


