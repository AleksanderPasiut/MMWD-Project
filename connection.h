#pragma once

#include "graphics.h"

class CONNECTION
{
	static ID2D1HwndRenderTarget* target;

	static void InitStaticFields(ID2D1HwndRenderTarget*);
	static void FreeStaticFields() noexcept;

	CONNECTION();
	CONNECTION(const CONNECTION&) = delete;
	CONNECTION& operator= (const CONNECTION&) = delete;

	void Paint() noexcept;

	friend class BOARD;
};