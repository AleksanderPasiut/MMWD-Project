#include "board.h"

void BOARD::ApplyDefaultExportPath(std::wstring& path) noexcept
{
	algorithm.ApplyDefaultExportPath(path);
}
void BOARD::LoadFromFile(std::fstream& File)
{
	Clear();

	algorithm.LoadFromFile(File);

	char buffer[2];
	File.read(buffer, 2);  // for debugging purposes

	size_t objects_amount;
	File.read(reinterpret_cast<char*>(&objects_amount), sizeof(size_t));
	for (size_t i = 0; i < objects_amount; i++)
	{
		D2D1_POINT_2F pos;
		double self_capabilities, self_need;
		File.read(reinterpret_cast<char*>(&pos), sizeof(D2D1_POINT_2F));
		File.read(reinterpret_cast<char*>(&self_capabilities), sizeof(double));
		File.read(reinterpret_cast<char*>(&self_need), sizeof(double));
		objects.push_back(new OBJECT(pos, self_need, self_capabilities));
	}	

	File.read(buffer, 2);  // for debugging purposes

	size_t pipe_types_amount;
	File.read(reinterpret_cast<char*>(&pipe_types_amount), sizeof(size_t));
	for (size_t i = 0; i < pipe_types_amount; i++)
	{
		double price, capacity;
		File.read(reinterpret_cast<char*>(&capacity), sizeof(double));
		File.read(reinterpret_cast<char*>(&price), sizeof(double));
		pipe_types.push_back(new PIPE_TYPE(capacity, price));
	}

	File.read(buffer, 2);  // for debugging purposes

	size_t connections_amount;
	File.read(reinterpret_cast<char*>(&connections_amount), sizeof(size_t));
	for (size_t i = 0; i < connections_amount; i++)
	{
		unsigned obj_source_id = 0;
		unsigned obj_target_id = 0;
		unsigned pipe_id = 0;

		File.read(reinterpret_cast<char*>(&obj_source_id), sizeof(unsigned));
		File.read(reinterpret_cast<char*>(&obj_target_id), sizeof(unsigned));
		File.read(reinterpret_cast<char*>(&pipe_id), sizeof(unsigned));

		if (obj_source_id >= objects_amount || obj_target_id >= objects_amount || pipe_id >= pipe_types_amount)
			throw 0;

		connections.push_back(new CONNECTION(objects[obj_source_id], objects[obj_target_id], pipe_types[pipe_id]));
	}

	algorithm.RefreshTotalObjectValues(connections);
	RedrawWindow(target->GetHwnd(), 0, 0, RDW_INTERNALPAINT);
}
void BOARD::SaveToFile(std::fstream& File)
{
	algorithm.SaveToFile(File);

	File.write("ox", 2); // for debugging purposes

	size_t objects_amount = objects.size();
	File.write(reinterpret_cast<const char*>(&objects_amount), sizeof(size_t));
	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		File.write(reinterpret_cast<const char*>(&(*it)->pos), sizeof(D2D1_POINT_2F));
		File.write(reinterpret_cast<const char*>(&(*it)->self_capabilities), sizeof(double));
		File.write(reinterpret_cast<const char*>(&(*it)->self_need), sizeof(double));
	}

	File.write("px", 2); // for debugging purposes

	size_t pipe_types_amount = pipe_types.size();
	File.write(reinterpret_cast<const char*>(&pipe_types_amount), sizeof(size_t));
	for (auto it = pipe_types.begin(); it != pipe_types.end(); it++)
	{
		File.write(reinterpret_cast<const char*>(&(*it)->capacity), sizeof(double));
		File.write(reinterpret_cast<const char*>(&(*it)->price), sizeof(double));
	}

	File.write("cx", 2); // for debugging purposes

	size_t connections_amount = connections.size();
	File.write(reinterpret_cast<const char*>(&connections_amount), sizeof(size_t));
	for (auto it = connections.begin(); it != connections.end(); it++)
	{
		unsigned i = 0;
		for (auto jt = objects.begin(); jt != objects.end(); jt++, i++)
			if (*jt == (*it)->obj_source)
			{
				File.write(reinterpret_cast<const char*>(&i), sizeof(unsigned));
				break;
			}

		i = 0;
		for (auto jt = objects.begin(); jt != objects.end(); jt++, i++)
			if (*jt == (*it)->obj_target)
			{
				File.write(reinterpret_cast<const char*>(&i), sizeof(unsigned));
				break;
			}

		i = 0;
		for (auto jt = pipe_types.begin(); jt != pipe_types.end(); jt++, i++)
			if (*jt == (*it)->pipe)
			{
				File.write(reinterpret_cast<const char*>(&i), sizeof(unsigned));
				break;
			}
	}
}