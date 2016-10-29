#pragma once

#include "graphics.h"

class OBJECT
{
	static ID2D1HwndRenderTarget* target;
	static ID2D1SolidColorBrush* brush_default;
	static IDWriteTextFormat* font_default;
	static float radius;
	static float stroke;
	static bool info;
	static void InitStaticFields(ID2D1HwndRenderTarget*, IDWriteFactory*);
	static void FreeStaticFields() noexcept;

	// pola znacz¹ce od strony algorytmu
	D2D1_POINT_2F pos;
	float self_need;
	float self_capabilities;
	float total_need;
	float total_capabilities;

	OBJECT(D2D1_POINT_2F pos, float self_need, float self_capabilities);
	OBJECT(const OBJECT&) = delete;
	OBJECT& operator= (const OBJECT&) = delete;

	void Paint() noexcept;

	friend class BOARD;
	friend class DIALOG_ADD_MODIFY;
	friend class MAIN_WINDOW;
	friend class MAIN_WINDOW_MENU;
};