#include <fstream>
#include "board.h"

void BOARD::LaunchTest() noexcept
{
	EnableWindow(target->GetHwnd(), false);
	DWORD ThreadId;
	algorithm.hThread = CreateThread(0, 0, TestThreadProc, this, 0, &ThreadId);
}

DWORD WINAPI TestThreadProc(void* arg) noexcept
{
	using namespace std;
	BOARD& board = *reinterpret_cast<BOARD*>(arg);

	wstring export_file = board.algorithm.export_path;

	fstream FS(export_file, std::fstream::out);

	board.algorithm.max_iterations = 1000;
	board.algorithm.export_path = wstring(L"");

	for (size_t taboo = 100; taboo <= 200; taboo += 5)
	{
		for (double kf_factor = 1.0; kf_factor < 2.1; kf_factor += 0.5)
		{
			board.algorithm.taboo_max_size = taboo;
			board.algorithm.kf = kf_factor;
			board.algorithm.Core();

			FS << taboo << " ";
			FS << kf_factor << " ";
			FS << board.algorithm.SolutionCost(board.connections) << " ";
			FS << board.algorithm.best_iteration << endl;
		}
	}
	
	board.algorithm.export_path = export_file;
	RedrawWindow(board.target->GetHwnd(), 0, 0, RDW_INTERNALPAINT);
	EnableWindow(board.target->GetHwnd(), true);
	CloseHandle(board.algorithm.hThread);
	board.algorithm.hThread = 0;
	return 0;
}