#include "solution.h"

SOLUTION::SOLUTION(const SOLUTION& arg)
{
	tab.resize(arg.tab.size());
	
	auto jt = arg.tab.begin();
	for (auto it = tab.begin(); it != tab.end(); it++, jt++)
		*it = new CONNECTION(**jt);
}
SOLUTION::SOLUTION(const std::vector<CONNECTION*>& arg)
{
	tab.resize(arg.size());
	
	auto jt = arg.begin();
	for (auto it = tab.begin(); it != tab.end(); it++, jt++)
		*it = new CONNECTION(**jt);
}
SOLUTION& SOLUTION::operator= (const SOLUTION& arg)
{
	if (tab.size())
		for (auto it = tab.begin(); it != tab.end(); it++)
			delete *it;
	
	tab.resize(arg.tab.size());
	
	auto jt = arg.tab.begin();
	for (auto it = tab.begin(); it != tab.end(); it++, jt++)
		*it = new CONNECTION(**jt);

	return *this;
}
SOLUTION& SOLUTION::operator= (const std::vector<CONNECTION*>& arg)
{
	if (tab.size())
		for (auto it = tab.begin(); it != tab.end(); it++)
			delete *it;
	
	tab.resize(arg.size());
	
	auto jt = arg.begin();
	for (auto it = tab.begin(); it != tab.end(); it++, jt++)
		*it = new CONNECTION(**jt);

	return *this;
}
SOLUTION::~SOLUTION() noexcept
{
	for (auto it = tab.begin(); it != tab.end(); it++)
		delete *it;
}

void SOLUTION::Export(std::vector<CONNECTION*>& out) const noexcept
{
	if (out.size())
		for (auto it = out.begin(); it != out.end(); it++)
			delete *it;
	
	out.resize(tab.size());
	
	auto jt = tab.begin();
	for (auto it = out.begin(); it != out.end(); it++, jt++)
		*it = new CONNECTION(**jt);
}