#include <string>
#include "connection.h"

CONNECTION::CONNECTION(OBJECT* obj_1, OBJECT* obj_2, PIPE_TYPE* pipe) : obj_1(obj_1), obj_2(obj_2), pipe(pipe)
{

}

double CONNECTION::Cost() const noexcept
{
	double objdx = obj_1->pos.x - obj_2->pos.x;
	double objdy = obj_1->pos.y - obj_2->pos.y;
	double distance = sqrt(objdx*objdx + objdy*objdy);
	return distance*pipe->price;
}

void CONNECTION::Paint() noexcept
{
	using namespace std;
	using namespace D2D1;

	float dy1 = 3;
	float dy2 = 17;

	target->DrawLine(obj_1->pos, obj_2->pos, brush_default, 1.3f);

	if (info)
	{
		float objdx = obj_1->pos.x - obj_2->pos.x;
		float objdy = obj_1->pos.y - obj_2->pos.y;
		float distance = sqrt(objdx*objdx + objdy*objdy);
		D2D1_POINT_2F center = Point2F((obj_1->pos.x+obj_2->pos.x)/2, (obj_1->pos.y+obj_2->pos.y)/2);

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