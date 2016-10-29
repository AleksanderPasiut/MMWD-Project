#include "io.h"

IO::IO(WNDPROC proc) : mainWindow(&graphics, proc), board(0)
{

}
IO::~IO() 
{

}
void IO::SetBoard(BOARD* board) noexcept
{
	this->board = board;

	mainWindow.SetBoard(board);
}
void IO::EventProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	mainWindow.EventProc(hwnd, uMsg, wParam, lParam);

	switch(uMsg)
	{
		case WM_PAINT:
		{
			if (hwnd == mainWindow.hwnd)
			{
				mainWindow.BeginDraw();

				if (board)
					board->Paint();

				mainWindow.EndDraw();
			}
			break;
		}
	}
}