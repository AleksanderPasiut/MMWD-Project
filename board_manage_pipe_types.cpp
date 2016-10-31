#include "board.h"
#include "dialog_manage_pipe_types.h"

void BOARD::ManagePipeTypes() noexcept
{
	DialogBoxParam(0,
				   L"dialog_manage_pipe_types",
				   target->GetHwnd(),
				   reinterpret_cast<DLGPROC>(DialogManagePipeTypes),
				   reinterpret_cast<LPARAM>(&pipe_types));
}