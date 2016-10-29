#include <string>
#include "dialog_add_modify.h"

struct OBJECT_PROPERTIES
{
	float pos_x;
	float pos_y;
	float self_need;
	float self_capabilities;
};

void InitDialog(HWND hwnd, const OBJECT_PROPERTIES* op) noexcept
{
	SetDlgItemTextW(hwnd,
					CTRL_EDIT_POS_X,
					std::to_wstring(op->pos_x).c_str());
	SetDlgItemTextW(hwnd,
					CTRL_EDIT_POS_Y,
					std::to_wstring(op->pos_y).c_str());
	SetDlgItemTextW(hwnd,
					CTRL_EDIT_SELF_NEED,
					std::to_wstring(op->self_need).c_str());
	SetDlgItemTextW(hwnd,
					CTRL_EDIT_SELF_CAPABILITIES,
					std::to_wstring(op->self_capabilities).c_str());
}
void SaveDialog(HWND hwnd, OBJECT_PROPERTIES* op)
{
	wchar_t buffer[30];
	GetDlgItemTextW(hwnd, CTRL_EDIT_POS_X, buffer, 30);
	op->pos_x = std::stof(std::wstring(buffer));
	GetDlgItemTextW(hwnd, CTRL_EDIT_POS_Y, buffer, 30);
	op->pos_y = std::stof(std::wstring(buffer));
	GetDlgItemTextW(hwnd, CTRL_EDIT_SELF_NEED, buffer, 30);
	op->self_need = std::stof(std::wstring(buffer));
	GetDlgItemTextW(hwnd, CTRL_EDIT_SELF_CAPABILITIES, buffer, 30);
	op->self_capabilities = std::stof(std::wstring(buffer));
}
BOOL CALLBACK DialogAddModifyProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static OBJECT_PROPERTIES* op = 0;

	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			op = reinterpret_cast<OBJECT_PROPERTIES*>(lParam);
			InitDialog(hwnd, op);
			break;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case CTRL_OK:
				{
					SaveDialog(hwnd, op);
					EndDialog(hwnd, 1);
					break;
				}
				case CTRL_CANCEL:
				{
					EndDialog(hwnd, 0);
					break;
				}
			}
			break;
		}
	}
	return FALSE;
}

void DIALOG_ADD_MODIFY::SetBoard(BOARD* board) noexcept
{
	this->board = board;
}
bool CheckingOPValues(HWND hwnd, const OBJECT_PROPERTIES* op)
{
	if (op->self_capabilities < 0)
	{
		MessageBoxW(hwnd, L"Wydajnoœæ nie mo¿e byæ ujemna.", L"B³¹d", MB_OK);
		return true;
	}
	if (op->self_need < 0)
	{
		MessageBoxW(hwnd, L"Zapotrzebowanie nie mo¿e byæ ujemne.", L"B³¹d", MB_OK);
		return true;
	}
	if (op->self_capabilities == 0 && op->self_need == 0)
	{
		MessageBoxW(hwnd, L"Zapotrzebowanie i wydajnoœæ nie mog¹ byæ jednoczeœnie zerowe.", L"B³¹d", MB_OK);
		return true;
	}
	if (op->self_capabilities != 0 && op->self_need != 0)
	{
		MessageBoxW(hwnd, L"Zapotrzebowanie i wydajnoœæ nie mog¹ byæ jednoczeœnie niezerowe.", L"B³¹d", MB_OK);
		return true;
	}

	return false;
}
void DIALOG_ADD_MODIFY::Dialog(HWND hwnd, D2D1_POINT_2F lastRClick) const noexcept
{
	if (!board)
		return;

	OBJECT_PROPERTIES op;

	if (board->selected)
	{
		op.pos_x = board->selected->pos.x;
		op.pos_y = board->selected->pos.y;
		op.self_need = board->selected->self_need;
		op.self_capabilities = board->selected->self_capabilities;
	}
	else
	{
		op.pos_x = lastRClick.x;
		op.pos_y = lastRClick.y;
		op.self_need = 0;
		op.self_capabilities = 0;
	}

	if (DialogBoxParamW(0,
						L"dialog_add_modify",
						hwnd,
						reinterpret_cast<DLGPROC>(DialogAddModifyProc),
						reinterpret_cast<LPARAM>(&op)))
	{
		if (CheckingOPValues(hwnd, &op))
			return;

		if (board->selected)
		{
			board->selected->pos.x = op.pos_x;
			board->selected->pos.y = op.pos_y;
			board->selected->self_need = op.self_need;
			board->selected->self_capabilities = op.self_capabilities;
		}
		else
		{
			board->NewObject(new OBJECT(D2D1::Point2F(op.pos_x, op.pos_y), op.self_need, op.self_capabilities));
		}

		RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
	}
}