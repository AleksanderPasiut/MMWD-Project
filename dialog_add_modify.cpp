#include <string>
#include "dialog_add_modify.h"

DIALOG_ADD_MODIFY_LPARAM* DIALOG_ADD_MODIFY::daml = 0;
void DIALOG_ADD_MODIFY::InitDialog(HWND hwnd) noexcept
{
	using namespace std;

	SetDlgItemText(hwnd, CTRL_EDIT_POS_X, to_wstring(daml->pos_x).c_str());
	SetDlgItemText(hwnd, CTRL_EDIT_POS_Y, to_wstring(daml->pos_y).c_str());
	SetDlgItemText(hwnd, CTRL_EDIT_SELF_NEED, to_wstring(daml->self_need).c_str());
	SetDlgItemText(hwnd, CTRL_EDIT_SELF_CAPABILITIES, to_wstring(daml->self_capabilities).c_str());
}
void DIALOG_ADD_MODIFY::ProcessEditTextPosX(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept
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
void DIALOG_ADD_MODIFY::ProcessEditTextPosY(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept
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
void DIALOG_ADD_MODIFY::ProcessEditTextSelfNeed(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept
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
void DIALOG_ADD_MODIFY::ProcessEditTextSelfCapabilities(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept
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
void DIALOG_ADD_MODIFY::ProcessEndDialog(HWND hwnd) noexcept
{
	try
	{
		using namespace std;

		wchar_t buffer[30];
		GetDlgItemTextW(hwnd, CTRL_EDIT_POS_X, buffer, 30);
		daml->pos_x = stof(wstring(buffer));
		GetDlgItemTextW(hwnd, CTRL_EDIT_POS_Y, buffer, 30);
		daml->pos_y = stof(wstring(buffer));
		GetDlgItemTextW(hwnd, CTRL_EDIT_SELF_NEED, buffer, 30);
		daml->self_need = stof(wstring(buffer));
		GetDlgItemTextW(hwnd, CTRL_EDIT_SELF_CAPABILITIES, buffer, 30);
		daml->self_capabilities = stof(wstring(buffer));

		if (daml->self_need < 0)
			MessageBox(hwnd, L"Zapotrzebowanie nie mo¿e byæ ujemne.", L"B³¹d", MB_OK);
		else if (daml->self_capabilities < 0)
			MessageBox(hwnd, L"Wydajnoœæ nie mo¿e byæ ujemna.", L"B³¹d", MB_OK);
		else if (daml->self_capabilities == 0 && daml->self_need == 0)
			MessageBox(hwnd, L"Zapotrzebowanie i wydajnoœæ nie mog¹ byæ jednoczeœnie zerowe.", L"B³¹d", MB_OK);
		else if (daml->self_capabilities != 0 && daml->self_need != 0)
			MessageBox(hwnd, L"Zapotrzebowanie i wydajnoœæ nie mog¹ byæ jednoczeœnie niezerowe.", L"B³¹d", MB_OK);
		else EndDialog(hwnd, 1);
	}
	catch(...)
	{
		MessageBox(hwnd, L"B³¹d wprowadzonej wartoœci.", L"B³¹d", MB_OK);
		return;
	}
}
BOOL CALLBACK DialogAddModify(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			DIALOG_ADD_MODIFY::daml = reinterpret_cast<DIALOG_ADD_MODIFY_LPARAM*>(lParam);
			DIALOG_ADD_MODIFY::InitDialog(hwnd);
			break;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case CTRL_EDIT_POS_X: DIALOG_ADD_MODIFY::ProcessEditTextPosX(hwnd, wParam, lParam); break;
				case CTRL_EDIT_POS_Y: DIALOG_ADD_MODIFY::ProcessEditTextPosY(hwnd, wParam, lParam); break;
				case CTRL_EDIT_SELF_NEED: DIALOG_ADD_MODIFY::ProcessEditTextSelfNeed(hwnd, wParam, lParam); break;
				case CTRL_EDIT_SELF_CAPABILITIES: DIALOG_ADD_MODIFY::ProcessEditTextSelfCapabilities(hwnd, wParam, lParam); break;
				case CTRL_OK: DIALOG_ADD_MODIFY::ProcessEndDialog(hwnd); break;
				case CTRL_CANCEL: EndDialog(hwnd, 0); break;
			}
			break;
		}
	}
	return FALSE;
}
