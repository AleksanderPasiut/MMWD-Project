#include "board.h"

void BOARD::RefreshTotalObjectValues() noexcept
{

}
bool BOARD::IsSolutionAcceptable() const noexcept
{
	for (auto it = objects.begin(); it != objects.end(); it++)
		if ((*it)->Ok() == false)
			return false;

	return true;
}
double BOARD::SolutionCost() const noexcept
{
	double ret = 0;

	for (auto it = connections.begin(); it != connections.end(); it++)
		ret += (*it)->Cost();

	return ret;
}
void BOARD::ConstructAlgorithm() noexcept
{

}
void BOARD::TabooAlgorithm() noexcept
{

}