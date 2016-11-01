#include <Windows.h>

#include "program.h"

PROGRAM* Program = 0;

LRESULT CALLBACK EventProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (Program)
		return Program->EventProc(hwnd, uMsg, wParam, lParam);
	else return DefWindowProc(hwnd, uMsg, wParam, lParam);
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR str, int nCmdShow)
{
	try
	{
		Program = new PROGRAM(EventProc);
	}
	catch(...) { return 0; }

	MSG msg;
	while(GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete Program;
	return static_cast<int>(msg.wParam);
}