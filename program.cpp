#include "program.h"

PROGRAM::PROGRAM(WNDPROC proc) : 
	Io(proc), 
	Board(Io.MainWindowTarget(), Io.graphics.dwriteFactory)
{
	Io.SetBoard(&Board);
}
PROGRAM::~PROGRAM() noexcept
{

}

LRESULT PROGRAM::EventProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	Io.EventProc(hwnd, uMsg, wParam, lParam);
	Board.EventProc(hwnd, uMsg, wParam, lParam);

	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}
