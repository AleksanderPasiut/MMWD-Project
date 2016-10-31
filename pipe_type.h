#pragma once

class PIPE_TYPE
{
	float capacity; // przepustowoœæ
	float price; // cena w jednostkach waluty / jednostkê odleg³oœci

	PIPE_TYPE(float capacity, float price) : capacity(capacity), price(price) {}
	PIPE_TYPE(const PIPE_TYPE&) = delete;
	PIPE_TYPE& operator= (const PIPE_TYPE&) = delete;

	friend class CONNECTION;
	friend class BOARD;
	friend class DIALOG_MANAGE_PIPE_TYPES;
};