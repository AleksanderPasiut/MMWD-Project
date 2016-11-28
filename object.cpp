#include <string>
#include "object.h"

OBJECT::OBJECT(D2D1_POINT_2F pos, double self_need, double self_capabilities) : 
	pos(pos), 
	self_need(self_need),
	self_capabilities(self_capabilities),
	total_need(self_need),
	total_capabilities(self_capabilities)
{}


double OBJECT::DistanceTo(const OBJECT& arg) const noexcept
{
	return sqrt((arg.pos.x - pos.x)*(arg.pos.x - pos.x) + (arg.pos.y - pos.y)*(arg.pos.y - pos.y));
}
bool OBJECT::Ok() const noexcept
{
	return total_need <= total_capabilities;
}
void OBJECT::Paint() noexcept
{
	using namespace std;
	using namespace D2D1;

	const float ix = 85;
	const float iy = 10;
	const float dx = 15;
	const float dy = 14;

	if (info)
	{
		wstring wstrX = wstring(L"x = ")+to_wstring(pos.x);
		target->DrawTextW(wstrX.c_str(), static_cast<UINT32>(wstrX.length()), font_default,
						  RectF(pos.x-dx, pos.y+iy, pos.x+ix, pos.y+iy+dy), brush_default);

		wstring wstrY = wstring(L"y = ")+to_wstring(pos.y);
		target->DrawTextW(wstrY.c_str(), static_cast<UINT32>(wstrY.length()), font_default,
						  RectF(pos.x-dx, pos.y+iy+dy, pos.x+ix, pos.y+iy+2*dy), brush_default);
	}

	wstring wstrV;
	if (self_need == 0)
	{
		// stacja
		target->DrawLine(Point2F(pos.x-radius, pos.y-radius),
						 Point2F(pos.x+radius, pos.y+radius), brush_default, stroke);
		target->DrawLine(Point2F(pos.x-radius, pos.y+radius),
						 Point2F(pos.x+radius, pos.y-radius), brush_default, stroke);

		if (info)
			wstrV = wstring(L"wyd.: ")+to_wstring(self_capabilities);
	}
	else if (self_capabilities == 0)
	{
		// miasto
		target->DrawEllipse(Ellipse(pos, radius, radius), brush_default, stroke);

		if (info)
			wstrV = wstring(L"zap.: ")+to_wstring(self_need);
	}

	if (info)
	{
		target->DrawTextW(wstrV.c_str(), static_cast<UINT32>(wstrV.length()), font_default,
						  RectF(pos.x-dx, pos.y+iy+2*dy, pos.x+ix, pos.y+iy+3*dy), brush_default);
	}
}