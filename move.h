#pragma once

#include "connection.h"

enum MOVE_TYPE
{
	MOVE_ADD,
	MOVE_DELETE
};

class MOVE
{
	CONNECTION arg1;
	CONNECTION arg2;

	void Assign(MOVE_TYPE move_type, const CONNECTION* arg) noexcept;
	void Assign(const CONNECTION* arg1, const CONNECTION* arg2) noexcept;

	bool operator== (const MOVE&) const noexcept;

	friend class BOARD;
};