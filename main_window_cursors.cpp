#include "main_window.h"

void MAIN_WINDOW::InitCursors()
{
	hCursor[MWCT_ARROW] = LoadCursor(0, IDC_ARROW);
	hCursor[MWCT_MOVE] = LoadCursor(0, IDC_SIZEALL);
	hCursor[MWCT_SELECT] = LoadCursor(0, IDC_HAND);
}
void MAIN_WINDOW::SetCursor(MW_CURSOR_TYPE cursor_type) const noexcept
{
	::SetCursor(hCursor[cursor_type]);
}
void MAIN_WINDOW::FreeCursors() noexcept
{
	for (unsigned i = 0; i < 3; i++)
		DeleteObject(hCursor[i]);
}