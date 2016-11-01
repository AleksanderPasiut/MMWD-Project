#pragma once

#include <Windows.h>
#include "graphics.h"
#include "main_window_menu.h"
#include "main_window_popup_menu.h"
#include "view_management.h"
#include "dialog_add_modify.h"
#include "file_manager.h"

enum MW_CURSOR_TYPE
{
	MWCT_ARROW = 0,
	MWCT_MOVE = 1,
	MWCT_SELECT = 2,
};

class MAIN_WINDOW
{
	GRAPHICS* graphics;
	MAIN_WINDOW_MENU mwMenu;
	MAIN_WINDOW_POPUP_MENU mwPopupMenu;
	VIEW_MANAGEMENT viewManagement;
	DIALOG_ADD_MODIFY dialogAddModify;
	WNDCLASSEXW wc;
	HWND hwnd;
	ID2D1HwndRenderTarget* target;
	BOARD* board;

	D2D1_POINT_2F lastRClick;
	bool object_moving;

	HCURSOR hCursor[3];
	void InitCursors();
	void FreeCursors() noexcept;
	void SetCursor(MW_CURSOR_TYPE) const noexcept;

	MAIN_WINDOW(GRAPHICS*, WNDPROC wndproc);
	MAIN_WINDOW(const MAIN_WINDOW&) = delete;
	MAIN_WINDOW& operator= (const MAIN_WINDOW&) = delete;
	~MAIN_WINDOW() noexcept;

	void SetBoard(BOARD* board) noexcept;
	void MoveObject(WPARAM wParam, LPARAM lParam) noexcept;
	void ClearTable() noexcept;
	void ShowGridAction() noexcept;
	void ShowObjectInfoAction() noexcept;
	void ShowConnectionsInfoAction() noexcept;
	void EventProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
	void UpdateTargetSize() noexcept;
	void UpdateTargetSize(LPARAM lParam) noexcept;
	void BeginDraw() noexcept;
	void EndDraw() noexcept;
	D2D1_POINT_2F LParamToLogicPt(LPARAM lParam) const noexcept;

	friend class IO;
};