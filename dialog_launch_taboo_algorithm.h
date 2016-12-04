#pragma once

#include <Windows.h>
#include <string>

#define CTRL_NUMB 0
#define CTRL_EDIT_TEXT_KF 0x31
#define CTRL_EDIT_TEXT_TABOO_MAX_SIZE 0x32
#define CTRL_EDIT_TEXT_MAX_ITERATIONS 0x33
#define CTRL_OK 0x01

struct DIALOG_LAUNCH_TABOO_ALGORITHM_LPARAM
{
	double* kf;
	size_t* taboo_max_size;
	size_t* max_iterations;
};

class DIALOG_LAUNCH_TABOO_ALGORITHM
{
	static DIALOG_LAUNCH_TABOO_ALGORITHM_LPARAM dltal;

	static void InitDialog(HWND hwnd) noexcept;
	static void ProcessEditTextKf(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEditTextTabooMaxSize(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEditTextMaxIterations(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEndDialog(HWND hwnd) noexcept;

	friend BOOL CALLBACK DialogLaunchTabooAlgorithm(HWND, UINT, WPARAM, LPARAM) noexcept;
};
