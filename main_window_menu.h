#pragma once

#include <Windows.h>
#include "board.h"

#define MWM_MANAGE_PIPE_TYPES 0x101
#define MWM_MANAGE_PUMPING_SYSTEM_COST 0x102
#define MWM_LAUNCH_TABOO_ALGORITHM 0x103
#define MWM_PRESENT_SOLUTION_DETAILS 0x104
#define MWM_LAUNCH_TEST 0x105
#define MWM_SHOW_GRID 0x201
#define MWM_SHOW_INFO_OBJECTS 0x202
#define MWM_SHOW_INFO_CONNECTIONS 0x203
#define MWM_FILE_NEW 0x301
#define MWM_FILE_OPEN 0x302
#define MWM_FILE_SAVE 0x303
#define MWM_FILE_SAVE_AS 0x304
#define MWM_QUIT 0x305

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
	friend class FILE_MANAGER;
};