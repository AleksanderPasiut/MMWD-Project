#include "board.h"

void BOARD::LoadFromFile(std::fstream& File)
{
	Clear();

	auto p = File.tellg();

	size_t objects_amount;
	File.read(reinterpret_cast<char*>(&objects_amount), sizeof(size_t));
	p = File.tellg();
	for (size_t i = 0; i < objects_amount; i++)
	{
		D2D1_POINT_2F pos;
		float self_capabilities, self_need;
		File.read(reinterpret_cast<char*>(&pos), sizeof(D2D1_POINT_2F));
		p = File.tellg();
		File.read(reinterpret_cast<char*>(&self_capabilities), sizeof(float));
		p = File.tellg();
		File.read(reinterpret_cast<char*>(&self_need), sizeof(float));
		p = File.tellg();
		objects.push_back(new OBJECT(pos, self_need, self_capabilities));	
	}	

	size_t pipe_types_amount;
	File.read(reinterpret_cast<char*>(&pipe_types_amount), sizeof(size_t));
	for (size_t i = 0; i < pipe_types_amount; i++)
	{
		float price, capacity;
		File.read(reinterpret_cast<char*>(&capacity), sizeof(float));
		File.read(reinterpret_cast<char*>(&price), sizeof(float));
		pipe_types.push_back(new PIPE_TYPE(capacity, price));
	}

	size_t connections_amount;
	File.read(reinterpret_cast<char*>(&connections_amount), sizeof(size_t));
	for (size_t i = 0; i < connections_amount; i++)
	{
		unsigned obj_1_id, obj_2_id, pipe_id;
		File.read(reinterpret_cast<char*>(&obj_1_id), sizeof(unsigned));
		File.read(reinterpret_cast<char*>(&obj_2_id), sizeof(unsigned));
		File.read(reinterpret_cast<char*>(&pipe_id), sizeof(unsigned));
		connections.push_back(new CONNECTION(objects[obj_1_id], objects[obj_2_id], pipe_types[pipe_id]));
	}
}
void BOARD::SaveToFile(std::fstream& File)
{
	size_t objects_amount = objects.size();
	File.write(reinterpret_cast<const char*>(&objects_amount), sizeof(size_t));
	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		File.write(reinterpret_cast<const char*>(&(*it)->pos), sizeof(D2D1_POINT_2F));
		File.write(reinterpret_cast<const char*>(&(*it)->self_capabilities), sizeof(float));
		File.write(reinterpret_cast<const char*>(&(*it)->self_need), sizeof(float));
	}

	size_t pipe_types_amount = pipe_types.size();
	File.write(reinterpret_cast<const char*>(&pipe_types_amount), sizeof(size_t));
	for (auto it = pipe_types.begin(); it != pipe_types.end(); it++)
	{
		File.write(reinterpret_cast<const char*>(&(*it)->capacity), sizeof(float));
		File.write(reinterpret_cast<const char*>(&(*it)->price), sizeof(float));
	}

	size_t connections_amount = connections.size();
	File.write(reinterpret_cast<const char*>(&connections_amount), sizeof(size_t));
	for (auto it = connections.begin(); it != connections.end(); it++)
	{
		unsigned i = 0;
		for (auto jt = objects.begin(); jt != objects.end(); jt++, i++)
		{
			if (*jt == (*it)->obj_1)
				File.write(reinterpret_cast<const char*>(&i), sizeof(unsigned));

			if (*jt == (*it)->obj_2)
				File.write(reinterpret_cast<const char*>(&i), sizeof(unsigned));
		}

		i = 0;
		for (auto jt = pipe_types.begin(); jt != pipe_types.end(); jt++, i++)
			if (*jt == (*it)->pipe)
			{
				File.write(reinterpret_cast<const char*>(&i), sizeof(unsigned));
				break;
			}
	}

	auto p = File.tellg();
	p = p;
}