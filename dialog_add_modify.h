#pragma once

#include <Windows.h>
#include "dialog_definitions.h"

struct DIALOG_ADD_MODIFY_LPARAM
{
	float pos_x;
	float pos_y;
	double self_need;
	double self_capabilities;
};

class DIALOG_ADD_MODIFY
{
	static DIALOG_ADD_MODIFY_LPARAM* daml;

	static void InitDialog(HWND hwnd) noexcept;
	static void ProcessEditTextPosX(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEditTextPosY(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEditTextSelfNeed(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEditTextSelfCapabilities(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEndDialog(HWND hwnd) noexcept;

	friend BOOL CALLBACK DialogAddModify(HWND, UINT, WPARAM, LPARAM) noexcept;
};

