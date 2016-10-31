#pragma once

#include <Windows.h>
#include "board.h"

#define PMB_ADD_OBJECT 0x1101
#define PM_CLEAR_TABLE 0x1102
#define PMA_MODIFY 0x1103
#define PMA_DELETE 0x1104

class MAIN_WINDOW_POPUP_MENU
{
	BOARD* board;

	HMENU hMotherMenuBlank;
	HMENU hMenuBlank;
	HMENU hMotherMenuActive;
	HMENU hMenuActive;

	MAIN_WINDOW_POPUP_MENU();
	MAIN_WINDOW_POPUP_MENU(const MAIN_WINDOW_POPUP_MENU&) = delete;
	MAIN_WINDOW_POPUP_MENU& operator= (const MAIN_WINDOW_POPUP_MENU&) = delete;
	~MAIN_WINDOW_POPUP_MENU() noexcept;

	void SetBoard(BOARD*) noexcept;
	void Mouse(HWND hwnd, WPARAM wParam, LPARAM lParam, D2D1_POINT_2F pt) noexcept;

	friend class MAIN_WINDOW;
};