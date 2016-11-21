#include <Windowsx.h>
#include "view_management.h"

VIEW_MANAGEMENT::VIEW_MANAGEMENT() : target(0), shift({0,0}), zoom(1), zoom_speed(1.1f)
{
	UpdateMatrix();
}
void VIEW_MANAGEMENT::SetTarget(ID2D1HwndRenderTarget* target) noexcept
{
	this->target = target;
}
void VIEW_MANAGEMENT::UpdateMatrix() noexcept
{
	using namespace D2D1;

	D2D1_SIZE_F ts;;

	if (target)
		ts = target->GetSize();
	else ts = SizeF();

	logicToPhysic = Matrix3x2F::Scale(SizeF(zoom, zoom)) *
					Matrix3x2F::Translation(SizeF(ts.width/2+shift.x, ts.height/2+shift.y));
	physicToLogic = logicToPhysic;
	physicToLogic.Invert();
}
void VIEW_MANAGEMENT::Zooming(WPARAM wParam, LPARAM lParam) noexcept
{
	POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	ScreenToClient(target->GetHwnd(), &point);

	short delta = GET_WHEEL_DELTA_WPARAM(wParam);
	D2D1_POINT_2F ps = { static_cast<float>(point.x), static_cast<float>(point.y) };
	D2D1_POINT_2F pl = { physicToLogic._11*ps.x + physicToLogic._21*ps.y + physicToLogic._31, 
						 physicToLogic._12*ps.x + physicToLogic._22*ps.y + physicToLogic._32 };
	float dzoom;
	if (delta > 0)
		dzoom = zoom_speed;
	else dzoom = 1/zoom_speed;

	zoom *= dzoom;

	shift.x += pl.x*(zoom / dzoom - zoom);
	shift.y += pl.y*(zoom / dzoom - zoom);

	UpdateMatrix();
}
void VIEW_MANAGEMENT::MovingStart(WPARAM wParam, LPARAM lParam) noexcept
{
	active = true;
	click = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	init_shift = shift;
}
bool VIEW_MANAGEMENT::MovingPerform(WPARAM wParam, LPARAM lParam) noexcept
{
	if (active)
	{
		shift.x = init_shift.x + static_cast<float>(GET_X_LPARAM(lParam) - click.x);
		shift.y = init_shift.y + static_cast<float>(GET_Y_LPARAM(lParam) - click.y);

		UpdateMatrix();
	}

	return active;
}
void VIEW_MANAGEMENT::MovingEnd(WPARAM wParam, LPARAM lParam) noexcept
{
	active = false;
}