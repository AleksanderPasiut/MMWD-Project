#pragma once

#include <Windows.h>
#include "io.h"
#include "board.h"

class PROGRAM
{
	static HANDLE hEvent;
	static bool PreOpen() noexcept;

	IO Io;
	BOARD Board;

	PROGRAM(WNDPROC proc);
	PROGRAM(const PROGRAM&) = delete;
	PROGRAM& operator= (const PROGRAM&) = delete;
	~PROGRAM() noexcept;

	LRESULT EventProc(HWND, UINT, WPARAM, LPARAM) noexcept;

	friend LRESULT CALLBACK EventProc(HWND, UINT, WPARAM, LPARAM);
	friend int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
};