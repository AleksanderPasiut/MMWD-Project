#include "dialog_launch_taboo_algorithm.h"

DIALOG_LAUNCH_TABOO_ALGORITHM_LPARAM DIALOG_LAUNCH_TABOO_ALGORITHM::dltal = {};
void DIALOG_LAUNCH_TABOO_ALGORITHM::InitDialog(HWND hwnd) noexcept
{
	SetDlgItemText(hwnd, CTRL_EDIT_TEXT_KF, std::to_wstring(*dltal.kf).c_str());
	SetDlgItemText(hwnd, CTRL_EDIT_TEXT_TABOO_MAX_SIZE, std::to_wstring(*dltal.taboo_max_size).c_str());
	SetDlgItemText(hwnd, CTRL_EDIT_TEXT_MAX_ITERATIONS, std::to_wstring(*dltal.max_iterations).c_str());
	SetDlgItemText(hwnd, CTRL_EDIT_TEXT_EXPORT_PATH, dltal.export_path->c_str());
}
void DIALOG_LAUNCH_TABOO_ALGORITHM::ProcessEditTextKf(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept
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
void DIALOG_LAUNCH_TABOO_ALGORITHM::ProcessEditTextTabooMaxSize(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept
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
void DIALOG_LAUNCH_TABOO_ALGORITHM::ProcessEditTextMaxIterations(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept
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
void DIALOG_LAUNCH_TABOO_ALGORITHM::ProcessEditTextExportPath(HWND hwnd, WPARAM wParam, LPARAM lParam) noexcept
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
void DIALOG_LAUNCH_TABOO_ALGORITHM::ProcessEndDialog(HWND hwnd) noexcept
{
	wchar_t buffer[30];
	GetDlgItemTextW(hwnd, CTRL_EDIT_TEXT_KF, buffer, 30);
	double kf = std::stod(std::wstring(buffer));
	GetDlgItemTextW(hwnd, CTRL_EDIT_TEXT_TABOO_MAX_SIZE, buffer, 30);
	size_t taboo_max_size = std::stoll(std::wstring(buffer));
	GetDlgItemTextW(hwnd, CTRL_EDIT_TEXT_MAX_ITERATIONS, buffer, 30);
	size_t max_iterations = std::stoll(std::wstring(buffer));

	int length = GetWindowTextLengthW(GetDlgItem(hwnd, CTRL_EDIT_TEXT_EXPORT_PATH))+1;
	std::wstring export_path(length, 0);
	GetDlgItemTextW(hwnd, CTRL_EDIT_TEXT_EXPORT_PATH, const_cast<wchar_t*>(export_path.c_str()), length);

	if (kf < 0)
		MessageBox(hwnd, L"Wartoœæ wspó³czynnika kary musi byæ dodatnia.", L"B³¹d", MB_OK);
	else if (taboo_max_size < 0)
		MessageBox(hwnd, L"Rozmiar listy taboo nie mo¿e byæ ujemny.", L"B³¹d", MB_OK);
	else if (max_iterations < 0)
		MessageBox(hwnd, L"Maksymalna liczba iteracji algorytmu musi byæ dodatnia.", L"B³¹d", MB_OK);
	else
	{
		*dltal.kf = kf;
		*dltal.taboo_max_size = taboo_max_size;
		*dltal.max_iterations = max_iterations;
		*dltal.export_path = export_path;
		EndDialog(hwnd, 0);
	}
}

BOOL CALLBACK DialogLaunchTabooAlgorithm(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			DIALOG_LAUNCH_TABOO_ALGORITHM::dltal = *reinterpret_cast<DIALOG_LAUNCH_TABOO_ALGORITHM_LPARAM*>(lParam);
			DIALOG_LAUNCH_TABOO_ALGORITHM::InitDialog(hwnd);
			break;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case CTRL_EDIT_TEXT_KF: DIALOG_LAUNCH_TABOO_ALGORITHM::ProcessEditTextKf(hwnd, wParam, lParam); break;
				case CTRL_EDIT_TEXT_TABOO_MAX_SIZE: DIALOG_LAUNCH_TABOO_ALGORITHM::ProcessEditTextTabooMaxSize(hwnd, wParam, lParam); break;
				case CTRL_EDIT_TEXT_MAX_ITERATIONS: DIALOG_LAUNCH_TABOO_ALGORITHM::ProcessEditTextMaxIterations(hwnd, wParam, lParam); break;
				case CTRL_EDIT_TEXT_EXPORT_PATH: DIALOG_LAUNCH_TABOO_ALGORITHM::ProcessEditTextExportPath(hwnd, wParam, lParam); break;
				case CTRL_OK: DIALOG_LAUNCH_TABOO_ALGORITHM::ProcessEndDialog(hwnd); break;
			}
			break;
		}
	}
	return FALSE;
}