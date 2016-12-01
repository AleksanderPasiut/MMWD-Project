#pragma once

#include <vector>
#include "connection.h"

class SOLUTION
{
	std::vector<CONNECTION*> tab;

	SOLUTION() {}
	SOLUTION(const SOLUTION&);
	SOLUTION(const std::vector<CONNECTION*>&);
	SOLUTION& operator= (const SOLUTION&);
	SOLUTION& operator= (const std::vector<CONNECTION*>&);
	~SOLUTION() noexcept;

	void Export(std::vector<CONNECTION*>& out) const noexcept;

	friend class BOARD;
};