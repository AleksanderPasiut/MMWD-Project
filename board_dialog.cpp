#include "board.h"
#include "dialog_manage_pipe_types.h"
#include "dialog_manage_pumping_system_cost.h"
#include "dialog_launch_taboo_algorithm.h"

void BOARD::ManagePipeTypes() noexcept
{
	DIALOG_MANAGE_PIPE_TYPES_LPARAM dmptl = { &pipe_types, &connections };

	DialogBoxParam(0,
				   L"dialog_manage_pipe_types",
				   target->GetHwnd(),
				   reinterpret_cast<DLGPROC>(DialogManagePipeTypes),
				   reinterpret_cast<LPARAM>(&dmptl));

	RedrawWindow(target->GetHwnd(), 0, 0, RDW_INTERNALPAINT);
}
void BOARD::ManagePumpingSystemCost() noexcept
{
	DIALOG_MANAGE_PUMPING_SYSTEM_COST_LPARAM dmpscl = { &g1, &g2 };

	DialogBoxParam(0,
				   L"dialog_manage_pumping_system_cost",
				   target->GetHwnd(),
				   reinterpret_cast<DLGPROC>(DialogManagePumpingSystemCost),
				   reinterpret_cast<LPARAM>(&dmpscl));

	RedrawWindow(target->GetHwnd(), 0, 0, RDW_INTERNALPAINT);
}
void BOARD::LaunchTabooAlgorithm() noexcept
{
	DIALOG_LAUNCH_TABOO_ALGORITHM_LPARAM dltal = { &kf, &taboo_max_size, &max_iterations };

	DialogBoxParam(0,
				   L"dialog_launch_taboo_algorithm",
				   target->GetHwnd(),
				   reinterpret_cast<DLGPROC>(DialogLaunchTabooAlgorithm),
				   reinterpret_cast<LPARAM>(&dltal));

	EnableWindow(target->GetHwnd(), false);
	DWORD ThreadId;
	algorithmThread = CreateThread(0, 0, AlgorithmThreadProc, this, 0, &ThreadId);
}
DWORD WINAPI AlgorithmThreadProc(void* arg) noexcept
{
	BOARD& board = *reinterpret_cast<BOARD*>(arg);
	board.TabooAlgorithmCore();
	EnableWindow(board.target->GetHwnd(), true);
	CloseHandle(board.algorithmThread);
	board.algorithmThread = 0;
	return 0;
}
void BOARD::BreakAlgorithm() noexcept
{
	TerminateThread(algorithmThread, 0);
	progressBar.Hide();
	EnableWindow(target->GetHwnd(), true);
	SetWindowPos(target->GetHwnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	algorithmThread = 0;
}
void BOARD::PresentSolutionDetails() noexcept
{
	using namespace std;
	RefreshTotalObjectValues(connections);
	double cost = SolutionCost(connections);
	double curry = kf*func_cT()*OutOfAcceptance();
	wstring text = wstring(L"Koszt bie¿¹cego rozwi¹zania wynosi: ")+to_wstring(cost)+wstring(L"\n");
	text += wstring(L"Wartoœæ funkcji kary: ")+to_wstring(curry);
	MessageBox(target->GetHwnd(), text.c_str(), L"Szczegó³y bie¿¹cego rozwi¹zania", MB_OK);
}