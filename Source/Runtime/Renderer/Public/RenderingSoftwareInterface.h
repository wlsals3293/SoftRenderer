
#pragma once

#include "DisplaySetting.h"
#include "LinearColor.h"
#include "Vector2.h"


class RenderingSoftwareInterface
{
public:
	virtual void Init(const bool InbSRGBColorSpace = true) = 0;
	virtual void Shutdown() = 0;
	virtual bool IsIntialized() const = 0;
	virtual bool IsSRGBSpace() const = 0;

	virtual void Clear(const LinearColor& InClearColor) = 0;
	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual void DrawTriangle(const Vector2& P1, const Vector2& P2, const Vector2& P3) = 0;
};
