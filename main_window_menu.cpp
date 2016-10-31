#include "main_window_menu.h"

MAIN_WINDOW_MENU::MAIN_WINDOW_MENU() : board(0)
{
	hMenu = LoadMenu(GetModuleHandle(0), L"main_window_menu");
}
MAIN_WINDOW_MENU::~MAIN_WINDOW_MENU() noexcept
{
	DestroyMenu(hMenu);
}

void MAIN_WINDOW_MENU::SetBoard(BOARD* board) noexcept
{
	this->board = board;
}
void MAIN_WINDOW_MENU::Update() noexcept
{
	CheckMenuItem(hMenu, MWM_SHOW_GRID, board->grid ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(hMenu, MWM_SHOW_INFO_OBJECTS, OBJECT::info ? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(hMenu, MWM_SHOW_INFO_CONNECTIONS, CONNECTION::info ? MF_CHECKED : MF_UNCHECKED);
}