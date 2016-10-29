#pragma once

#include <vector>
#include "graphics.h"
#include "object.h"
#include "connection.h"

class BOARD
{
	ID2D1HwndRenderTarget* target;

	std::vector<OBJECT*> objects;
	std::vector<CONNECTION*> connections;

	OBJECT* selected;
	bool grid;

	BOARD(ID2D1HwndRenderTarget*, IDWriteFactory*);
	BOARD(const BOARD&) = delete;
	BOARD& operator= (const BOARD&) = delete;
	~BOARD() noexcept;

	void EventProc(HWND, UINT, WPARAM, LPARAM) noexcept;
	void NewObject(OBJECT*) noexcept;
	void DeleteSelected() noexcept;
	void Clear() noexcept;

	void PaintGrid() const noexcept;
	void Paint() const noexcept;
	OBJECT* UpdateSelected(const D2D1_POINT_2F&) noexcept;
	
	friend class MAIN_WINDOW;
	friend class MAIN_WINDOW_MENU;
	friend class MAIN_WINDOW_POPUP_MENU;
	friend class DIALOG_ADD_MODIFY;
	friend class IO;
	friend class PROGRAM;
};