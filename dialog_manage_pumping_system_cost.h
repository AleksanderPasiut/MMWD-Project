#pragma once

#include <Windows.h>
#include <string>
#include "dialog_definitions.h"

struct DIALOG_MANAGE_PUMPING_SYSTEM_COST_LPARAM
{
	double* lp_g1;
	double* lp_g2;
};

class DIALOG_MANAGE_PUMPING_SYSTEM_COST
{
	static DIALOG_MANAGE_PUMPING_SYSTEM_COST_LPARAM dmpscl;

	static void InitDialog(HWND hwnd) noexcept;
	static void ProcessEditTextG1(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEditTextG2(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEndDialog(HWND hwnd) noexcept;

	friend BOOL CALLBACK DialogManagePumpingSystemCost(HWND, UINT, WPARAM, LPARAM) noexcept;
};
