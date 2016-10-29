#pragma once

#include "graphics.h"
#include "main_window.h"
#include "board.h"

class IO
{
	GRAPHICS graphics;
	MAIN_WINDOW mainWindow;

	BOARD* board;

	IO(WNDPROC proc);
	IO(const IO&) = delete;
	IO& operator= (const IO&) = delete;
	~IO() noexcept;

	void SetBoard(BOARD*) noexcept;

	void Paint() noexcept;
	void EventProc(HWND, UINT, WPARAM, LPARAM) noexcept;

	ID2D1HwndRenderTarget* MainWindowTarget() const noexcept { return mainWindow.target; }

	friend class PROGRAM;
};


