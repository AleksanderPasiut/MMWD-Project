#pragma once

#include <Windows.h>
#include <string>
#include "dialog_definitions.h"

struct DIALOG_LAUNCH_TABOO_ALGORITHM_LPARAM
{
	double* kf;
	size_t* taboo_max_size;
	size_t* max_iterations;
	std::wstring* export_path;
};

class DIALOG_LAUNCH_TABOO_ALGORITHM
{
	static DIALOG_LAUNCH_TABOO_ALGORITHM_LPARAM dltal;

	static void InitDialog(HWND hwnd) noexcept;
	static void ProcessEditTextKf(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEditTextTabooMaxSize(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEditTextMaxIterations(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEditTextExportPath(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEndDialog(HWND hwnd) noexcept;

	friend BOOL CALLBACK DialogLaunchTabooAlgorithm(HWND, UINT, WPARAM, LPARAM) noexcept;
};
