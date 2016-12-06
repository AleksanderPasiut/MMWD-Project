#include "move.h"

void MOVE::Assign(MOVE_TYPE move_type, const CONNECTION* arg) noexcept
{
	switch(move_type)
	{
		case MOVE_ADD:
		{
			arg1.obj_source = 0;
			arg2 = *arg;
			break;
		}
		case MOVE_DELETE:
		{
			arg1 = *arg;
			arg2.obj_source = 0;
			break;
		}
	}
}
void MOVE::Assign(const CONNECTION* arg1, const CONNECTION* arg2) noexcept
{
	this->arg1 = *arg1;
	this->arg2 = *arg2;
}

bool MOVE::operator== (const MOVE& arg) const noexcept
{
	if (!arg1.obj_source)
	{
		if (arg.arg1.obj_source)
			return false;

		 return arg2 == arg.arg2;
	}
	else if (!arg2.obj_source)
	{
		if (arg.arg2.obj_source)
			return false;

		return arg1 == arg.arg1;
	}
	else 
		return (arg1 == arg.arg1) && (arg2 == arg.arg2);
}