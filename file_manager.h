#pragma once

#include <Windows.h>

#include "object.h"
#include "pipe_type.h"
#include "connection.h"
#include "board.h"
#include "view_management.h"
#include "main_window_menu.h"

class FILE_MANAGER
{
	static const int MAX_PATH_LENGTH = 35000;
	static wchar_t fileOpened[MAX_PATH_LENGTH];
	static HWND hwnd;
	static BOARD* board;
	static VIEW_MANAGEMENT* viewManagement;
	static MAIN_WINDOW_MENU* mainWindowMenu;

	static OPENFILENAMEW openFileName;
	
	static void GetFilePathFromCommandLine() noexcept;
	static void UpdateMainWindowText() noexcept;
	static BOOL OpenFileDialog() noexcept;
	static void ApplyOpenedFile() noexcept;
	static BOOL SaveFileDialog() noexcept;
	static void SaveToFile() noexcept;

	static void InitFileManager(HWND, BOARD*, VIEW_MANAGEMENT*, MAIN_WINDOW_MENU*);
	static void NewFile() noexcept;
	static void OpenFile() noexcept;
	static void SaveFile() noexcept;
	static void SaveFileAs() noexcept;
	static void FreeFileManager() noexcept;

	friend class MAIN_WINDOW;
	friend class PROGRAM;
};