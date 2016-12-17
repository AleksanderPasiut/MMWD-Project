#include "board.h"
#include "dialog_add_modify.h"
#include "dialog_manage_pipe_types.h"
#include "dialog_manage_pumping_system_cost.h"
#include "dialog_launch_taboo_algorithm.h"

void BOARD::AddAtPoint(D2D1_POINT_2F pt) noexcept
{
	DIALOG_ADD_MODIFY_LPARAM daml = { pt.x, pt.y, 0, 0 };

	if (DialogBoxParam(0,
					   L"dialog_add_modify",
					   target->GetHwnd(),
					   reinterpret_cast<DLGPROC>(DialogAddModify),
					   reinterpret_cast<LPARAM>(&daml)))
	{
		NewObject(new OBJECT(D2D1::Point2F(daml.pos_x, daml.pos_y), daml.self_need, daml.self_capabilities));
	}
}
void BOARD::ModifySelected() noexcept
{
	if (!selected)
		return;

	DIALOG_ADD_MODIFY_LPARAM daml = { selected->pos.x, selected->pos.y, selected->self_need, selected->self_capabilities };

	if (DialogBoxParam(0,
					   L"dialog_add_modify",
					   target->GetHwnd(),
					   reinterpret_cast<DLGPROC>(DialogAddModify),
					   reinterpret_cast<LPARAM>(&daml)))
	{
		selected->pos.x = daml.pos_x;
		selected->pos.y = daml.pos_y;
		selected->self_need = daml.self_need;
		selected->self_capabilities = daml.self_capabilities;
	}
}
void BOARD::ManagePipeTypes() noexcept
{
	DIALOG_MANAGE_PIPE_TYPES_LPARAM dmptl = { &pipe_types, &connections };

	DialogBoxParam(0,
				   L"dialog_manage_pipe_types",
				   target->GetHwnd(),
				   reinterpret_cast<DLGPROC>(DialogManagePipeTypes),
				   reinterpret_cast<LPARAM>(&dmptl));
}
void BOARD::ManagePumpingSystemCost() noexcept
{
	DIALOG_MANAGE_PUMPING_SYSTEM_COST_LPARAM dmpscl = { &algorithm.g1, &algorithm.g2 };

	DialogBoxParam(0,
				   L"dialog_manage_pumping_system_cost",
				   target->GetHwnd(),
				   reinterpret_cast<DLGPROC>(DialogManagePumpingSystemCost),
				   reinterpret_cast<LPARAM>(&dmpscl));
}
bool BOARD::CheckTabooAlgorithmLaunch() const noexcept
{
	if (objects.size() == 0)
	{
		MessageBoxW(target->GetHwnd(), L"Nie ma ¿adnych obiektów do po³¹czenia.", L"B³¹d", MB_OK);
		return true;
	}

	if (pipe_types.size() == 0)
	{
		MessageBoxW(target->GetHwnd(), L"Nie ma ¿adnych rodzajów rur.", L"B³¹d", MB_OK);
		return true;
	}

	return false;
}
void BOARD::LaunchTabooAlgorithm() noexcept
{
	if (CheckTabooAlgorithmLaunch())
		return;

	DIALOG_LAUNCH_TABOO_ALGORITHM_LPARAM dltal = 
	{
		&algorithm.kf,
		&algorithm.taboo_max_size,
		&algorithm.max_iterations,
		&algorithm.export_path
	};

	if (DialogBoxParam(0,
					   L"dialog_launch_taboo_algorithm",
					   target->GetHwnd(),
					   reinterpret_cast<DLGPROC>(DialogLaunchTabooAlgorithm),
					   reinterpret_cast<LPARAM>(&dltal)))
	{
		EnableWindow(target->GetHwnd(), false);
		DWORD ThreadId;
		algorithm.hThread = CreateThread(0, 0, AlgorithmThreadProc, this, 0, &ThreadId);
	}
}
DWORD WINAPI AlgorithmThreadProc(void* arg) noexcept
{
	BOARD& board = *reinterpret_cast<BOARD*>(arg);
	board.algorithm.Core();
	RedrawWindow(board.target->GetHwnd(), 0, 0, RDW_INTERNALPAINT);
	EnableWindow(board.target->GetHwnd(), true);
	board.PresentBestIteration();
	CloseHandle(board.algorithm.hThread);
	board.algorithm.hThread = 0;
	return 0;
}
void BOARD::BreakAlgorithm() noexcept
{
	TerminateThread(algorithm.hThread, 0);
	progressBar.Hide();
	EnableWindow(target->GetHwnd(), true);
	SetWindowPos(target->GetHwnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	algorithm.hThread = 0;
}
void BOARD::PresentSolutionDetails() noexcept
{
	using namespace std;
	algorithm.RefreshTotalObjectValues(connections);
	wstring text = wstring(L"Koszt bie¿¹cego rozwi¹zania wynosi: ")+to_wstring(algorithm.SolutionCost(connections))+wstring(L"\n");
	text += wstring(L"Wartoœæ wspó³czynnika cT funkcji kary: ")+to_wstring(algorithm.func_cT())+wstring(L"\n");
	text += wstring(L"Ca³kowity rozmiar przekroczonych ograniczeñ: ")+to_wstring(algorithm.OutOfAcceptance())+wstring(L"\n");
	text += wstring(L"Indeks iteracji, w której uzyskane zosta³o najlepsze rozwi¹zanie: ") + to_wstring(algorithm.best_iteration);
	MessageBox(target->GetHwnd(), text.c_str(), L"Szczegó³y bie¿¹cego rozwi¹zania", MB_OK);
}
void BOARD::PresentBestIteration() noexcept
{
	using namespace std;
	wstring text = wstring(L"Indeks iteracji, w której uzyskane zosta³o najlepsze rozwi¹zanie: ") + to_wstring(algorithm.best_iteration);
	MessageBox(target->GetHwnd(), text.c_str(), L"", MB_OK);
}