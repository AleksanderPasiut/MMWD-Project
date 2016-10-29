#pragma once

#include <Windows.h>
#include "board.h"

#define CTRL_NUMB 0
#define CTRL_EDIT_POS_X 0x11
#define CTRL_EDIT_POS_Y 0x12
#define CTRL_EDIT_SELF_NEED 0x13
#define CTRL_EDIT_SELF_CAPABILITIES 0x14
#define CTRL_CANCEL 0x01
#define CTRL_OK 0x02

class DIALOG_ADD_MODIFY
{
	BOARD* board;

	DIALOG_ADD_MODIFY() : board(0) {}
	DIALOG_ADD_MODIFY(const DIALOG_ADD_MODIFY&) = delete;
	DIALOG_ADD_MODIFY& operator= (const DIALOG_ADD_MODIFY&) = delete;

	void SetBoard(BOARD*) noexcept;
	void Dialog(HWND, D2D1_POINT_2F) const noexcept;

	friend class MAIN_WINDOW;
};

