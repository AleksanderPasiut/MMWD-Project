#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include "pipe_type.h"
#include "connection.h"

#define CTRL_NUMB 0
#define CTRL_LISTBOX 0x11
#define CTRL_EDIT_TEXT_CAPACITY 0x12
#define CTRL_EDIT_TEXT_PRICE 0x13
#define CTRL_ADD_PIPE_TYPE 0x14
#define CTRL_DEL_PIPE_TYPE 0x15
#define CTRL_OK 0x01

struct DIALOG_MANAGE_PIPE_TYPES_LPARAM
{
	std::vector<PIPE_TYPE*>* pipe_types;
	std::vector<CONNECTION*>* connections;
};

class DIALOG_MANAGE_PIPE_TYPES
{
	static std::vector<PIPE_TYPE*>* pipe_types;
	static std::vector<CONNECTION*>* connections;

	static void InitDialog(HWND hwnd) noexcept;
	static void ProcessListbox(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEditTextCapacity(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessEditTextPrice(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessAddPipeType(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;
	static void ProcessDelPipeType(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept;

	friend BOOL CALLBACK DialogManagePipeTypes(HWND, UINT, WPARAM, LPARAM) noexcept;
};


