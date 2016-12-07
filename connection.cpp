#include <string>
#include "connection.h"

CONNECTION::CONNECTION(OBJECT* obj_source, OBJECT* obj_target, PIPE_TYPE* pipe) : 
	obj_source(obj_source), obj_target(obj_target), pipe(pipe) {}

bool CONNECTION::operator== (const CONNECTION& arg) const noexcept
{
	return obj_target == arg.obj_target && obj_source == arg.obj_source && pipe == arg.pipe;
}

double CONNECTION::Cost(double u1, double u2) const noexcept
{
	double objdx = obj_source->pos.x - obj_target->pos.x;
	double objdy = obj_source->pos.y - obj_target->pos.y;
	double distance = sqrt(objdx*objdx + objdy*objdy);
	return distance*pipe->price + u1*distance + u2*pipe->capacity;
}

void CONNECTION::Paint() noexcept
{
	using namespace std;
	using namespace D2D1;

	float dy1 = 2;
	float dy2 = 12;

	target->DrawLine(obj_source->pos, obj_target->pos, brush_default, 1.3f);

	if (info)
	{
		float objdx = obj_source->pos.x - obj_target->pos.x;
		float objdy = obj_source->pos.y - obj_target->pos.y;
		float distance = sqrt(objdx*objdx + objdy*objdy);
		D2D1_POINT_2F center = Point2F((obj_source->pos.x+obj_target->pos.x)/2, (obj_source->pos.y+obj_target->pos.y)/2);

		wstring top = wstring(L"przep. = ")+to_wstring(pipe->capacity)+wstring(L", odl. = ")+to_wstring(distance);
		wstring bottom = wstring(L"koszt = ")+to_wstring(distance*pipe->price);

		Matrix3x2F transform;
		target->GetTransform(&transform);

		Matrix3x2F text_transform = Matrix3x2F::Rotation(static_cast<float>(180/3.141529*atan(objdy/objdx)), center) * transform;

		target->SetTransform(&text_transform);

		target->DrawText(top.c_str(),
						 static_cast<UINT32>(top.length()),
						 font_default,
						 RectF(center.x-distance/2, center.y-dy2, center.x+distance/2, center.y-dy1),
						 brush_default);

		target->DrawText(bottom.c_str(),
						 static_cast<UINT32>(bottom.length()),
						 font_default,
						 RectF(center.x-distance/2, center.y+dy1, center.x+distance/2, center.y+dy2),
						 brush_default);

		target->SetTransform(&transform);
	}
}