#include <windowsx.h>
#include "main_window.h"

MAIN_WINDOW::MAIN_WINDOW(GRAPHICS* graphics, WNDPROC wndproc) : graphics(graphics), object_moving(false)
{
	ZeroMemory(&wc, sizeof(WNDCLASSEXW));
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.lpfnWndProc = wndproc;
	wc.lpszClassName = L"SimulatorMainWindowClass";
	wc.hInstance = GetModuleHandle(0);
	wc.hIcon = LoadIcon(GetModuleHandle(0), L"icon_big");
	wc.hIconSm = LoadIcon(GetModuleHandle(0), L"icon_small");
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));

	if (!RegisterClassExW(&wc))
		throw 0;

	try
	{
		hwnd = CreateWindowExW(0,
							   wc.lpszClassName,
							   0,
							   WS_OVERLAPPEDWINDOW,
							   CW_USEDEFAULT,
							   CW_USEDEFAULT,
							   CW_USEDEFAULT,
							   CW_USEDEFAULT,
							   0,
							   mwMenu.hMenu,
							   wc.hInstance,
							   0);
		if (!hwnd)
			throw 0;

		try
		{
			using namespace D2D1;

			if (FAILED(graphics->d2d1Factory->CreateHwndRenderTarget(RenderTargetProperties(),
																	 HwndRenderTargetProperties(hwnd, SizeU(1,1)),
																	 &target)))
				throw 0;

			try
			{
				InitCursors();
			}
			catch(...) { target->Release(); throw; }
		}
		catch(...) { DestroyWindow(hwnd); throw; }
	}
	catch(...) { UnregisterClassW(wc.lpszClassName, wc.hInstance); throw; }

	ShowWindow(hwnd, SW_MAXIMIZE);
	viewManagement.SetTarget(target);
	UpdateTargetSize();
}
MAIN_WINDOW::~MAIN_WINDOW() noexcept
{
	FreeCursors();
	target->Release();
	DestroyWindow(hwnd);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

void MAIN_WINDOW::SetBoard(BOARD* board) noexcept
{
	this->board = board;
	mwMenu.SetBoard(board);
	mwPopupMenu.SetBoard(board);
	dialogAddModify.SetBoard(board);
}
void MAIN_WINDOW::MoveObject(WPARAM wParam, LPARAM lParam) noexcept
{
	board->selected->pos = LParamToLogicPt(lParam);
	RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
}
void MAIN_WINDOW::ClearTable() noexcept
{
	board->Clear();
	RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
}
void MAIN_WINDOW::ShowGridAction() noexcept
{
	board->grid = !board->grid;
	mwMenu.Update();
	RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
}
void MAIN_WINDOW::ShowObjectInfoAction() noexcept
{
	OBJECT::info = !OBJECT::info;
	mwMenu.Update();
	RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
}
void MAIN_WINDOW::ShowConnectionsInfoAction() noexcept
{
	CONNECTION::info = !CONNECTION::info;
	mwMenu.Update();
	RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
}
void MAIN_WINDOW::EventProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN:
		{
			if (!object_moving && board->UpdateSelected(LParamToLogicPt(lParam)))
			{
				object_moving = true;
			}
			else
			{
				object_moving = false;
				viewManagement.MovingStart(wParam, lParam);
			}
			SetCapture(hwnd);
			RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (object_moving)
				MoveObject(wParam, lParam);
			else
			{
				if (board->UpdateSelected(LParamToLogicPt(lParam)))
					SetCursor(MWCT_SELECT);
				else SetCursor(MWCT_MOVE);

				if (viewManagement.MovingPerform(wParam, lParam))
					RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			ReleaseCapture();
			viewManagement.MovingEnd(wParam, lParam);
			RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			viewManagement.Zooming(wParam, lParam);
			RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			lastRClick = LParamToLogicPt(lParam);
			mwPopupMenu.Mouse(hwnd, wParam, lParam, lastRClick);
			break;
		}
		case WM_SHOWWINDOW:
		case WM_SIZE:
		{
			UpdateTargetSize();
			break;
		}
		case WM_SIZING:
		{
			UpdateTargetSize(lParam);
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		case WM_COMMAND:
		{
			if (HIWORD(wParam) == 0)
				switch(LOWORD(wParam))
				{
					case PMB_ADD_OBJECT: dialogAddModify.Dialog(hwnd, lastRClick); break;
					case PMA_MODIFY: dialogAddModify.Dialog(hwnd, lastRClick); break;
					case PMA_DELETE: board->DeleteSelected(); break;
					case PM_CLEAR_TABLE: ClearTable(); break;
					case MWM_MANAGE_PIPE_TYPES: board->ManagePipeTypes(); break;
					case MWM_CONSTRUCT_ALGORITHM: board->ConstructAlgorithm(); break;
					case MWM_TABOO_ALGORITHM: board->TabooAlgorithm(); break;
					case MWM_SHOW_GRID: ShowGridAction(); break;
					case MWM_SHOW_INFO_OBJECTS: ShowObjectInfoAction(); break;
					case MWM_SHOW_INFO_CONNECTIONS: ShowConnectionsInfoAction(); break;
					case MWM_FILE_NEW: FILE_MANAGER::NewFile(); break;
					case MWM_FILE_OPEN: FILE_MANAGER::OpenFile(); break;
					case MWM_FILE_SAVE: FILE_MANAGER::SaveFile(); break;
					case MWM_FILE_SAVE_AS: FILE_MANAGER::SaveFileAs(); break;
					case MWM_QUIT: PostQuitMessage(0); break;
				}
			break;
		}
	}
}
void MAIN_WINDOW::UpdateTargetSize() noexcept
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	target->Resize(D2D1::SizeU(rect.right-rect.left, rect.bottom-rect.top));
	viewManagement.UpdateMatrix();
	RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
}
void MAIN_WINDOW::UpdateTargetSize(LPARAM lParam) noexcept
{
	RECT& rect = *reinterpret_cast<RECT*>(lParam);
	target->Resize(D2D1::SizeU(rect.right-rect.left, rect.bottom-rect.top));
	viewManagement.UpdateMatrix();
	RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
}
void MAIN_WINDOW::BeginDraw() noexcept
{
	BeginPaint(hwnd, 0);
	target->BeginDraw();
	target->Clear(D2D1::ColorF(0.8f, 0.8f, 0.8f));

	target->SetTransform(viewManagement.logicToPhysic);
}
void MAIN_WINDOW::EndDraw() noexcept
{
	target->EndDraw();
	EndPaint(hwnd, 0);
}
D2D1_POINT_2F MAIN_WINDOW::LParamToLogicPt(LPARAM lParam) const noexcept
{
	float fx = static_cast<float>(GET_X_LPARAM(lParam));
	float fy = static_cast<float>(GET_Y_LPARAM(lParam));

	const D2D1::Matrix3x2F& m = viewManagement.physicToLogic;

	return D2D1::Point2F(m._11*fx + m._21*fy + m._31,
						 m._12*fx + m._22*fy + m._32);
}
