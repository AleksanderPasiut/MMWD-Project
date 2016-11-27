#include "board.h"
#include "dialog_manage_pipe_types.h"
#include "dialog_manage_pumping_system_cost.h"

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