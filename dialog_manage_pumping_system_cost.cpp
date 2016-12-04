#include "dialog_manage_pumping_system_cost.h"

DIALOG_MANAGE_PUMPING_SYSTEM_COST_LPARAM DIALOG_MANAGE_PUMPING_SYSTEM_COST::dmpscl = {};
void DIALOG_MANAGE_PUMPING_SYSTEM_COST::InitDialog(HWND hwnd) noexcept
{
	SetDlgItemText(hwnd, CTRL_EDIT_TEXT_G1, std::to_wstring(*dmpscl.lp_g1).c_str());
	SetDlgItemText(hwnd, CTRL_EDIT_TEXT_G2, std::to_wstring(*dmpscl.lp_g2).c_str());
}
void DIALOG_MANAGE_PUMPING_SYSTEM_COST::ProcessEditTextG1(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept
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
void DIALOG_MANAGE_PUMPING_SYSTEM_COST::ProcessEditTextG2(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept
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
void DIALOG_MANAGE_PUMPING_SYSTEM_COST::ProcessEndDialog(HWND hwnd) noexcept
{
	wchar_t buffer[30];
	GetDlgItemTextW(hwnd, CTRL_EDIT_TEXT_G1, buffer, 30);
	double g1 = std::stod(std::wstring(buffer));
	GetDlgItemTextW(hwnd, CTRL_EDIT_TEXT_G2, buffer, 30);
	double g2 = std::stod(std::wstring(buffer));

	if (g1 < 0 || g2 < 0)
		MessageBox(hwnd, L"Wartoœci wspó³czynników nie mog¹ byæ ujemne", L"B³¹d", MB_OK);
	else
	{
		*dmpscl.lp_g1 = g1;
		*dmpscl.lp_g2 = g2;
		EndDialog(hwnd, 0);
	}
}

BOOL CALLBACK DialogManagePumpingSystemCost(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			DIALOG_MANAGE_PUMPING_SYSTEM_COST::dmpscl = *reinterpret_cast<DIALOG_MANAGE_PUMPING_SYSTEM_COST_LPARAM*>(lParam);
			DIALOG_MANAGE_PUMPING_SYSTEM_COST::InitDialog(hwnd);
			break;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case CTRL_EDIT_TEXT_G1: DIALOG_MANAGE_PUMPING_SYSTEM_COST::ProcessEditTextG1(hwnd, wParam, lParam); break;
				case CTRL_EDIT_TEXT_G2: DIALOG_MANAGE_PUMPING_SYSTEM_COST::ProcessEditTextG2(hwnd, wParam, lParam); break;
				case CTRL_OK: DIALOG_MANAGE_PUMPING_SYSTEM_COST::ProcessEndDialog(hwnd); break;
			}
			break;
		}
	}
	return FALSE;
}