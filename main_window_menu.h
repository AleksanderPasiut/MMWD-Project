#pragma once

#include <Windows.h>
#include "board.h"

#define OPTION_1_1 1
#define OPTION_1_2 2
#define MWM_SHOW_GRID 3
#define MWM_SHOW_INFO 4

class MAIN_WINDOW_MENU
{
	BOARD* board;

	HMENU hMenu;

	MAIN_WINDOW_MENU();
	MAIN_WINDOW_MENU(const MAIN_WINDOW_MENU&) = delete;
	MAIN_WINDOW_MENU& operator= (const MAIN_WINDOW_MENU&) = delete;
	~MAIN_WINDOW_MENU() noexcept;

	void SetBoard(BOARD*) noexcept;
	void Update() noexcept;

	friend class MAIN_WINDOW;
};