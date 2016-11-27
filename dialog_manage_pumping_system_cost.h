#pragma once

#include <Windows.h>
#include <string>

#define CTRL_NUMB 0
#define CTRL_EDIT_TEXT_G1 0x21
#define CTRL_EDIT_TEXT_G2 0x22
#define CTRL_OK 0x01

struct DIALOG_MANAGE_PUMPING_SYSTEM_COST_LPARAM
{
	double* lp_g1;
	double* lp_g2;
};

class DIALOG_MANAGE_PUMPING_SYSTEM_COST
{
	static double* lp_g1;
	static double* lp_g2;

	static void InitDialog(HWND hwnd) noexcept;
	static void ProcessEditTextG1(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEditTextG2(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEndDialog(HWND hwnd) noexcept;

	friend BOOL CALLBACK DialogManagePumpingSystemCost(HWND, UINT, WPARAM, LPARAM) noexcept;
};
