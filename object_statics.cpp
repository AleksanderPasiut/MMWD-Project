#include "object.h"

ID2D1HwndRenderTarget* OBJECT::target = 0;
ID2D1SolidColorBrush* OBJECT::brush_default = 0;
IDWriteTextFormat* OBJECT::font_default = 0;
float OBJECT::radius = 0;
float OBJECT::stroke = 0;
bool OBJECT::info = true;

void OBJECT::InitStaticFields(ID2D1HwndRenderTarget* arg, IDWriteFactory* dWriteFactory)
{
	target = arg;

	if (FAILED(target->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &brush_default)))
		throw 0;

	try
	{
		if (FAILED(dWriteFactory->CreateTextFormat(L"Tahoma", NULL,
												   DWRITE_FONT_WEIGHT_NORMAL,
												   DWRITE_FONT_STYLE_NORMAL,
												   DWRITE_FONT_STRETCH_NORMAL,
												   13.0f, L"pl-pl", &font_default)))
			throw 0;
	}
	catch(...) { brush_default->Release(); throw; }

	radius = 10;
	stroke = 3;
}
void OBJECT::FreeStaticFields() noexcept
{
	font_default->Release();
	brush_default->Release();
}