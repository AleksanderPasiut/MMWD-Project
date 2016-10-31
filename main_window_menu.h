#pragma once

#include <Windows.h>
#include "board.h"

#define MWM_MANAGE_PIPE_TYPES 0x101
#define MWM_CONSTRUCT_ALGORITHM 0x102
#define MWM_TABOO_ALGORITHM 0x103
#define MWM_SHOW_GRID 0x201
#define MWM_SHOW_INFO_OBJECTS 0x202
#define MWM_SHOW_INFO_CONNECTIONS 0x203

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