#include "program.h"

PROGRAM::PROGRAM(WNDPROC proc) : 
	Io(proc), 
	Board(Io.MainWindowTarget(), Io.graphics.dwriteFactory)
{
	Io.SetBoard(&Board);
	
	FILE_MANAGER::InitFileManager(Io.MainWindowHwnd(),
									&Board,
									Io.MainWindowViewManagement(),
									Io.MainWindowMenu());
}
PROGRAM::~PROGRAM() noexcept
{
	FILE_MANAGER::FreeFileManager();
}

LRESULT PROGRAM::EventProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	Io.EventProc(hwnd, uMsg, wParam, lParam);
	
	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}
