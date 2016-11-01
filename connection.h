#pragma once

#include "graphics.h"
#include "object.h"
#include "pipe_type.h"

class CONNECTION
{
	static ID2D1HwndRenderTarget* target;
	static ID2D1SolidColorBrush* brush_default;
	static IDWriteTextFormat* font_default;
	static bool info;
	static void InitStaticFields(ID2D1HwndRenderTarget*, IDWriteFactory*);
	static void FreeStaticFields() noexcept;

	// pola znacz¹ce od strony algorytmu
	OBJECT* obj_1;
	OBJECT* obj_2;
	PIPE_TYPE* pipe;

	CONNECTION(OBJECT* obj_1, OBJECT* obj_2, PIPE_TYPE* pipe);
	CONNECTION(const CONNECTION&) = delete;
	CONNECTION& operator= (const CONNECTION&) = delete;

	void Paint() noexcept;

	friend class BOARD;
	friend class MAIN_WINDOW;
	friend class MAIN_WINDOW_MENU;
	friend class FILE_MANAGER;
};