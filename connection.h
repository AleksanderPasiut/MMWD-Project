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
	OBJECT* obj_source;
	OBJECT* obj_target;
	PIPE_TYPE* pipe;

	CONNECTION() {}
	CONNECTION(OBJECT* obj_source, OBJECT* obj_target, PIPE_TYPE* pipe);

	bool operator== (const CONNECTION&) const noexcept;
	double Cost(double u1, double u2) const noexcept;
	void Paint() noexcept;

	friend class MOVE;
	friend class SOLUTION;
	friend class ALGORITHM;
	friend class BOARD;
	friend class MAIN_WINDOW;
	friend class MAIN_WINDOW_MENU;
	friend class DIALOG_MANAGE_PIPE_TYPES;
	friend class FILE_MANAGER;
};