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
	D2D1_POINT_2F pos;	// po³o¿enie obiektu

	double self_need;	// zapotrzebowanie w³asne
	double self_capabilities;	// wydajnoœæ w³asna

	double total_need;	// zapotrzebowanie w³asne + zapotrzebowanie obiektów pod³¹czonych
	double total_capabilities; // wydajnoœæ w³asna + wydajnoœæ obiektów pod³¹czonych

	OBJECT(D2D1_POINT_2F pos, double self_need, double self_capabilities);
	OBJECT(const OBJECT&) = delete;
	OBJECT& operator= (const OBJECT&) = delete;

	bool Ok() const noexcept;
	void Paint() noexcept;

	friend class CONNECTION;
	friend class BOARD;
	friend class DIALOG_ADD_MODIFY;
	friend class MAIN_WINDOW;
	friend class MAIN_WINDOW_MENU;
	friend class FILE_MANAGER;
};