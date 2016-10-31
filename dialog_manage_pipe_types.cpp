#include "dialog_manage_pipe_types.h"

std::vector<PIPE_TYPE*>* DIALOG_MANAGE_PIPE_TYPES::table = 0;
void DIALOG_MANAGE_PIPE_TYPES::InitDialog(HWND hwnd) noexcept
{
	using namespace std;
	wstring wstr;

	for (auto it = table->begin(); it != table->end(); it++)
	{
		wstr = wstring(L"przep. = ")+to_wstring((*it)->capacity)+wstring(L", cena = ")+to_wstring((*it)->price);
		SendMessage(GetDlgItem(hwnd, CTRL_LISTBOX), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(wstr.c_str()));
	}

	SetDlgItemText(hwnd, CTRL_EDIT_TEXT_CAPACITY, to_wstring(static_cast<float>(0)).c_str());
	SetDlgItemText(hwnd, CTRL_EDIT_TEXT_PRICE, to_wstring(static_cast<float>(0)).c_str());
}
void DIALOG_MANAGE_PIPE_TYPES::ProcessListbox(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept
{
	switch(HIWORD(wParam))
	{
		case LBN_SELCHANGE:
		{
			float capacity = 0;
			float price = 0;
			int select = static_cast<int>(SendMessage(GetDlgItem(hwnd, CTRL_LISTBOX), LB_GETCURSEL, 0, 0));
			if (select != LB_ERR)
			{
				capacity = table->operator[](select)->capacity;
				price = table->operator[](select)->price;
			}
			SetDlgItemText(hwnd, CTRL_EDIT_TEXT_CAPACITY, std::to_wstring(capacity).c_str());
			SetDlgItemText(hwnd, CTRL_EDIT_TEXT_PRICE, std::to_wstring(price).c_str());
			break;
		}
	}
}
void DIALOG_MANAGE_PIPE_TYPES::ProcessEditTextCapacity(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept
{
	switch(HIWORD(wParam))
	{
		case EN_SETFOCUS:
		{
			PostMessage(GetDlgItem(hwnd, LOWORD(wParam)), EM_SETSEL, 0, -1);
			break;
		}
	}
}
void DIALOG_MANAGE_PIPE_TYPES::ProcessEditTextPrice(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept
{
	switch(HIWORD(wParam))
	{
		case EN_SETFOCUS:
		{
			PostMessage(GetDlgItem(hwnd, LOWORD(wParam)), EM_SETSEL, 0, -1);
			break;
		}
	}
}
void DIALOG_MANAGE_PIPE_TYPES::ProcessAddPipeType(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept
{
	using namespace std;

	wchar_t buffer[30];
	GetDlgItemTextW(hwnd, CTRL_EDIT_TEXT_CAPACITY, buffer, 30);
	float capacity = std::stof(std::wstring(buffer));
	GetDlgItemTextW(hwnd, CTRL_EDIT_TEXT_PRICE, buffer, 30);
	float price = std::stof(std::wstring(buffer));

	wstring wstr = wstring(L"przep. = ")+to_wstring(capacity)+wstring(L", cena = ")+to_wstring(price);

	int search = static_cast<int>(SendMessage(GetDlgItem(hwnd, CTRL_LISTBOX), LB_FINDSTRING, -1, reinterpret_cast<LPARAM>(wstr.c_str())));

	if (search == LB_ERR)
	{
		SendMessage(GetDlgItem(hwnd, CTRL_LISTBOX), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(wstr.c_str()));
		table->push_back(new PIPE_TYPE(capacity, price));
	}
}
void DIALOG_MANAGE_PIPE_TYPES::ProcessDelPipeType(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept
{
	int select = static_cast<int>(SendMessage(GetDlgItem(hwnd, CTRL_LISTBOX), LB_GETCURSEL, 0, 0));
	if (select != LB_ERR)
	{
		PIPE_TYPE* pipe = table->operator[](select);

		for (auto it = table->begin(); it != table->end(); it++)
			if (*it == pipe)
			{
				delete *it;
				table->erase(it);
				break;
			}

		SendMessage(GetDlgItem(hwnd, CTRL_LISTBOX), LB_DELETESTRING, select, 0);
	}
}

BOOL CALLBACK DialogManagePipeTypes(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			DIALOG_MANAGE_PIPE_TYPES::table = reinterpret_cast<std::vector<PIPE_TYPE*>*>(lParam);
			DIALOG_MANAGE_PIPE_TYPES::InitDialog(hwnd);
			break;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case CTRL_LISTBOX: DIALOG_MANAGE_PIPE_TYPES::ProcessListbox(hwnd, wParam, lParam); break;
				case CTRL_EDIT_TEXT_CAPACITY: DIALOG_MANAGE_PIPE_TYPES::ProcessEditTextCapacity(hwnd, wParam, lParam); break;
				case CTRL_EDIT_TEXT_PRICE: DIALOG_MANAGE_PIPE_TYPES::ProcessEditTextPrice(hwnd, wParam, lParam); break;
				case CTRL_ADD_PIPE_TYPE: DIALOG_MANAGE_PIPE_TYPES::ProcessAddPipeType(hwnd, wParam, lParam); break;
				case CTRL_DEL_PIPE_TYPE: DIALOG_MANAGE_PIPE_TYPES::ProcessDelPipeType(hwnd, wParam, lParam); break;
				case CTRL_OK: EndDialog(hwnd, 0); break;
			}
			break;
		}
	}
	return FALSE;
}