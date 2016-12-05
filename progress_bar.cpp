#include "progress_bar.h"

LRESULT CALLBACK ProgressBarProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_COMMAND:
		{
			switch(HIWORD(wParam))
			{
				case BN_CLICKED:
				{
					switch(LOWORD(wParam))
					{
						case MWM_BREAK_BUTTON:
						{
							PostMessage(reinterpret_cast<HWND>(GetWindowLongPtr(hwnd, GWLP_USERDATA)), WM_COMMAND, MWM_BREAK_BUTTON, 0);
							break;
						}
					}
					break;
				}
			}
			break;
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

PROGRESS_BAR::PROGRESS_BAR(HWND parent) 
{
	INITCOMMONCONTROLSEX icc = 
	{
		sizeof(INITCOMMONCONTROLSEX),
		ICC_PROGRESS_CLASS,
	};

	if (!InitCommonControlsEx(&icc))
		throw 0;

	SetThemeAppProperties(STAP_ALLOW_NONCLIENT | STAP_ALLOW_CONTROLS | STAP_ALLOW_WEBCONTENT);

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = ProgressBarProc;
	wc.lpszClassName = L"ProgressBarClass";
	wc.hIcon = 0;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	wc.hInstance = GetModuleHandle(0);
	
	if (!RegisterClassEx(&wc))
		throw 0;

	try
	{
		RECT rect = 
		{
			GetSystemMetrics(SM_CXSCREEN)/2-195,
			GetSystemMetrics(SM_CYSCREEN)/2-20,
			GetSystemMetrics(SM_CXSCREEN)/2+195,
			GetSystemMetrics(SM_CYSCREEN)/2+20,
		};

		AdjustWindowRectEx(&rect, WS_BORDER | WS_CAPTION, false, 0);

		hwnd = CreateWindowEx(0,
							  wc.lpszClassName,
							  0,
							  WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN,
							  rect.left,
							  rect.top,
							  rect.right - rect.left,
							  rect.bottom - rect.top,
							  parent,
							  0,
							  wc.hInstance,
							  0);
		if (!hwnd)
			throw 0;

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(parent));

		try
		{
			bar = CreateWindowExW(0,
								  PROGRESS_CLASSW,
								  0,
								  WS_CHILD | WS_VISIBLE,
								  10,
								  10,
								  280,
								  20,
								  hwnd,
								  0,
								  wc.hInstance,
								  0);
			if (!bar)
				throw 0;

			SetWindowTheme(bar, L"Explorer", NULL);

			try
			{
				stop = CreateWindowExW(0,
									   L"BUTTON",
									   L"Przerwij",
									   WS_CHILD | WS_VISIBLE,
									   300,
									   10,
									   80,
									   20,
									   hwnd,
									   reinterpret_cast<HMENU>(MWM_BREAK_BUTTON),
									   wc.hInstance,
									   0);
				if (!stop)
					throw 0;

				SendMessage(stop, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
			}
			catch(...) { DestroyWindow(bar); throw; }
		}
		catch(...) { DestroyWindow(hwnd); throw; }
	}
	catch(...) { UnregisterClass(wc.lpszClassName, wc.hInstance); throw; }
}
PROGRESS_BAR::~PROGRESS_BAR() noexcept
{
	DestroyWindow(stop);
	DestroyWindow(bar);
	DestroyWindow(hwnd);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

void PROGRESS_BAR::SetPos(size_t pos) noexcept
{
	using namespace std;
	SendMessage(bar, PBM_SETPOS, static_cast<WPARAM>(pos*100/range), 0);
	wstring text = wstring(L"Iteracja: ") + to_wstring(pos) + wstring(L" / ") + to_wstring(range);
	SetWindowText(hwnd, text.c_str());
}
void PROGRESS_BAR::SetRange(size_t range) noexcept
{
	this->range = range;
}
void PROGRESS_BAR::Show() const noexcept
{
	ShowWindow(hwnd, SW_SHOWNORMAL);
}
void PROGRESS_BAR::Hide() const noexcept
{
	ShowWindow(hwnd, SW_HIDE);
}