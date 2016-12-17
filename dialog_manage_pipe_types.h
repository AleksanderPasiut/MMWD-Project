#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include "pipe_type.h"
#include "connection.h"
#include "dialog_definitions.h"

struct DIALOG_MANAGE_PIPE_TYPES_LPARAM
{
	std::vector<PIPE_TYPE*>* pipe_types;
	std::vector<CONNECTION*>* connections;
};

class DIALOG_MANAGE_PIPE_TYPES
{
	static DIALOG_MANAGE_PIPE_TYPES_LPARAM dmptl;

	static void InitDialog(HWND hwnd) noexcept;
	static void ProcessListbox(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEditTextCapacity(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEditTextPrice(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessAddPipeType(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessDelPipeType(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void SortPipes() noexcept;
	
	friend BOOL CALLBACK DialogManagePipeTypes(HWND, UINT, WPARAM, LPARAM) noexcept;
};


