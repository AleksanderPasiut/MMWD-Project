#include "connection.h"

ID2D1HwndRenderTarget* CONNECTION::target = 0;
ID2D1SolidColorBrush* CONNECTION::brush_default = 0;
IDWriteTextFormat* CONNECTION::font_default = 0;
bool CONNECTION::info = true;

void CONNECTION::InitStaticFields(ID2D1HwndRenderTarget* arg, IDWriteFactory* dWriteFactory)
{
	target = arg;
	
	if (FAILED(target->CreateSolidColorBrush(D2D1::ColorF(0.4f, 0.0f, 0.0f), &brush_default)))
		throw 0;

	try
	{
		if (FAILED(dWriteFactory->CreateTextFormat(L"Tahoma", NULL,
												   DWRITE_FONT_WEIGHT_NORMAL,
												   DWRITE_FONT_STYLE_NORMAL,
												   DWRITE_FONT_STRETCH_NORMAL,
												   11.0f, L"pl-pl", &font_default)))
			throw 0;

		font_default->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	}
	catch(...) { brush_default->Release(); throw; }
}
void CONNECTION::FreeStaticFields() noexcept
{
	font_default->Release();
	brush_default->Release();
}