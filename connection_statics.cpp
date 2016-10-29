#include "connection.h"

ID2D1HwndRenderTarget* CONNECTION::target = 0;

void CONNECTION::InitStaticFields(ID2D1HwndRenderTarget* arg)
{
	target = arg;
}
void CONNECTION::FreeStaticFields() noexcept
{

}