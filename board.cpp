#include "board.h"

BOARD::BOARD(ID2D1HwndRenderTarget* target, IDWriteFactory* dWriteFactory) : target(target), grid(true)
{
	OBJECT::InitStaticFields(target, dWriteFactory);

	try
	{
		CONNECTION::InitStaticFields(target);
	}
	catch(...) { OBJECT::FreeStaticFields(); throw; }

	objects.push_back(new OBJECT(D2D1::Point2F(0.0f, 0.0f), 1, 0));
	objects.push_back(new OBJECT(D2D1::Point2F(150.0f, 0.0f), 0, 2));
}
BOARD::~BOARD() noexcept
{
	Clear();
	CONNECTION::FreeStaticFields();
	OBJECT::FreeStaticFields();
}

void BOARD::EventProc(HWND, UINT, WPARAM, LPARAM) noexcept
{

}
void BOARD::NewObject(OBJECT* new_object) noexcept
{
	objects.push_back(new_object);
}
void BOARD::DeleteSelected() noexcept
{
	for (auto it = objects.begin(); it != objects.end(); it++)
		if (*it == selected)
		{
			objects.erase(it);
			selected = 0;
			return;
		}
}
void BOARD::Clear() noexcept
{
	for (auto it = objects.begin(); it != objects.end(); it++)
		delete *it;

	objects.clear();

	for (auto it = connections.begin(); it != connections.end(); it++)
		delete *it;

	connections.clear();
}

void BOARD::PaintGrid() const noexcept
{
	using namespace D2D1;

	float interval = 100;

	D2D1_SIZE_F size = target->GetSize();
	Matrix3x2F t;
	target->GetTransform(&t);
	t.Invert();
	D2D1_RECT_F boundaries = RectF(t._31,
								   t._32,
								   size.width*t._11+size.height*t._21+t._31,
								   size.width*t._12+size.height*t._22+t._32);

	for (int i = static_cast<int>(boundaries.top/interval); i <= static_cast<int>(boundaries.bottom/interval); i++)
	{
		target->DrawLine(Point2F(boundaries.left, interval*i),
						 Point2F(boundaries.right, interval*i),
						 OBJECT::brush_default, 0.5f);
	}

	for (int i = static_cast<int>(boundaries.left/interval); i <= static_cast<int>(boundaries.right/interval); i++)
	{
		target->DrawLine(Point2F(interval*i, boundaries.top),
						 Point2F(interval*i, boundaries.bottom),
						 OBJECT::brush_default, 0.5f);
	}
}
void BOARD::Paint() const noexcept
{
	if (grid)
		PaintGrid();

	for (auto it = objects.begin(); it != objects.end(); it++)
		(*it)->Paint();

	for (auto it = connections.begin(); it != connections.end(); it++)
		(*it)->Paint();
}
OBJECT* BOARD::UpdateSelected(const D2D1_POINT_2F& pt) noexcept
{
	float sq_dist = HUGE_VALF;
	OBJECT* ret = 0;

	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		float new_sq_dist = (pt.x - (*it)->pos.x)*(pt.x - (*it)->pos.x) + (pt.y - (*it)->pos.y)*(pt.y - (*it)->pos.y);
		if (new_sq_dist < 4*OBJECT::radius*OBJECT::radius && new_sq_dist < sq_dist)
		{
			sq_dist = new_sq_dist;
			ret = *it;
		}
	}

	selected = ret;
	return ret;
}