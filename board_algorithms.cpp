#include <string>

#include "board.h"

void BOARD::RefreshTotalObjectValues() noexcept
{
	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		(*it)->total_capabilities = (*it)->self_capabilities;
		(*it)->total_need = (*it)->self_need;
	}

	for (auto it = connections.begin(); it != connections.end(); it++)
	{
		(*it)->obj_source->total_need += (*it)->pipe->capacity;
		(*it)->obj_target->total_capabilities += (*it)->pipe->capacity;
	}
}
bool BOARD::IsSolutionAcceptable() const noexcept
{
	for (auto it = objects.begin(); it != objects.end(); it++)
		if (!(*it)->Ok())
			return false;

	return true;
}
double BOARD::SolutionCost() const noexcept
{
	double ret = 0;

	for (auto it = connections.begin(); it != connections.end(); it++)
		ret += (*it)->Cost(g1, g2);

	return ret;
}
void BOARD::ClearConnections() noexcept
{
	for (auto it = connections.begin(); it != connections.end(); it++)
		delete *it;

	connections.clear();
}
void BOARD::PresentSolutionCost() const noexcept
{
	using namespace std;

	double cost = SolutionCost();
	wstring text = wstring(L"Koszt bie¿¹cego rozwi¹zania wynosi: ")+to_wstring(cost);
	MessageBox(target->GetHwnd(), text.c_str(), L"Koszt bie¿¹cego rozwi¹zania", MB_OK);
}
void BOARD::PresentSolutionAcceptability() const noexcept
{
	if (IsSolutionAcceptable())
		MessageBox(target->GetHwnd(), L"Bie¿¹ce rozwi¹zanie jest dopuszczalne.", L"Dopuszczalnoœæ rozwi¹zania", MB_OK);
	else MessageBox(target->GetHwnd(), L"Bie¿¹ce rozwi¹zanie nie jest dopuszczalne.", L"Dopuszczalnoœæ rozwi¹zania", MB_OK);
}
void BOARD::ConstructAlgorithmPasiut() noexcept
{
	ClearConnections();

	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		if ((*it)->self_capabilities)
			continue;

		double distance = HUGE_VAL;
		OBJECT* source = 0;
		for (auto jt = objects.begin(); jt != objects.end(); jt++)
		{
			if ((*jt)->self_need)
				continue;

			double new_distance = (*it)->DistanceTo(**jt);
			if (distance > new_distance)
			{
				distance = new_distance;
				source = *jt;
			}
		}

		PIPE_TYPE* pipe = 0;
		double needed_capacity = HUGE_VAL;
		for (auto jt = pipe_types.begin(); jt != pipe_types.end(); jt++)
		{
			if ((*jt)->capacity < (*it)->self_need)
				continue;

			if (needed_capacity > (*jt)->capacity)
			{
				needed_capacity = (*jt)->capacity;
				pipe = *jt;
			}
		}

		connections.push_back(new CONNECTION(source, *it, pipe));
	}

	RefreshTotalObjectValues();
}
void BOARD::ConstructAlgorithmTrojnarski() noexcept
{

}
void BOARD::TabooAlgorithm() noexcept
{

}