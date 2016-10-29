#include <windowsx.h>
#include "main_window_popup_menu.h"

MAIN_WINDOW_POPUP_MENU::MAIN_WINDOW_POPUP_MENU() : board(0)
{
	hMotherMenuBlank = LoadMenu(0, L"main_window_popup_menu_blank");
	hMenuBlank = GetSubMenu(hMotherMenuBlank, 0);
	hMotherMenuActive = LoadMenu(0, L"main_window_popup_menu_active");
	hMenuActive = GetSubMenu(hMotherMenuActive, 0);
}
MAIN_WINDOW_POPUP_MENU::~MAIN_WINDOW_POPUP_MENU() noexcept
{
	DestroyMenu(hMenuActive);
	DestroyMenu(hMotherMenuActive);
	DestroyMenu(hMenuBlank);
	DestroyMenu(hMotherMenuBlank);
}

void MAIN_WINDOW_POPUP_MENU::SetBoard(BOARD* board) noexcept
{
	this->board = board;
}
void MAIN_WINDOW_POPUP_MENU::Mouse(HWND hwnd, WPARAM wParam, LPARAM lParam, D2D1_POINT_2F pt) noexcept
{
	if (board->UpdateSelected(pt))
		TrackPopupMenu(hMenuActive, TPM_LEFTBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, hwnd, 0);
	else TrackPopupMenu(hMenuBlank, TPM_LEFTBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, hwnd, 0);
}