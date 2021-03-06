#pragma once

#include <Windows.h>
#include <CommCtrl.h>
#include <UxTheme.h>
#include <string>

#pragma comment(lib,"Comctl32.lib")
#pragma comment(lib,"UxTheme.lib")

#define MWM_BREAK_BUTTON 0x401

class PROGRESS_BAR
{
	WNDCLASSEX wc;
	HWND hwnd;
	HWND bar;
	HWND stop;

	size_t range;

	PROGRESS_BAR(HWND);
	PROGRESS_BAR(const PROGRESS_BAR&) = delete;
	PROGRESS_BAR& operator= (const PROGRESS_BAR&) = delete;
	~PROGRESS_BAR() noexcept;

	void SetPos(size_t) noexcept;
	void SetRange(size_t) noexcept;
	void Show() const noexcept;
	void Hide() const noexcept;

	friend class ALGORITHM;
	friend class BOARD;
};