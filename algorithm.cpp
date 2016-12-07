#include <string>
#include <fstream>

#include "solution.h"
#include "move.h"
#include "algorithm.h"

ALGORITHM::ALGORITHM(std::vector<OBJECT*>& objects,	std::vector<PIPE_TYPE*>& pipe_types, std::vector<CONNECTION*>& connections, PROGRESS_BAR& progressBar) :
	objects(objects),
	pipe_types(pipe_types),
	connections(connections),
	progressBar(progressBar),
	g1(0),
	g2(0),
	hThread(0),
	kf(1),
	taboo_max_size(400),
	best_iteration(0),
	max_iterations(2000)
{

}

void ALGORITHM::RefreshTotalObjectValues(const std::vector<CONNECTION*>& tab) noexcept
{
	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		(*it)->total_capabilities = (*it)->self_capabilities;
		(*it)->total_need = (*it)->self_need;
	}

	for (auto it = tab.begin(); it != tab.end(); it++)
	{
		(*it)->obj_source->total_need += (*it)->pipe->capacity;
		(*it)->obj_target->total_capabilities += (*it)->pipe->capacity;
	}
}
bool ALGORITHM::IsSolutionAcceptable() const noexcept
{
	for (auto it = objects.begin(); it != objects.end(); it++)
		if (!(*it)->Ok())
			return false;

	return true;
}
double ALGORITHM::SolutionCost(const std::vector<CONNECTION*>& tab) const noexcept
{
	double ret = 0;

	for (auto it = tab.begin(); it != tab.end(); it++)
		ret += (*it)->Cost(g1, g2);

	return ret;
}
void ALGORITHM::ClearConnections() noexcept
{
	for (auto it = connections.begin(); it != connections.end(); it++)
		delete *it;

	connections.clear();
}

double ALGORITHM::PipeCapacityToPrice(double capacity) const noexcept
{
	if (pipe_types.size() == 1)
		return (*pipe_types.begin())->price;

	// lower extrapolation
	if (capacity < (*pipe_types.begin())->capacity)
	{
		return (*pipe_types.begin())->price;
	}
	// upper extrapolation
	else if (capacity >= (*(pipe_types.end()-1))->capacity)
	{
		double alpha = ((*(pipe_types.end()-1))->price - (*(pipe_types.end()-2))->price)/
					   ((*(pipe_types.end()-1))->capacity - (*(pipe_types.end()-2))->capacity);

		return alpha * (capacity - (*(pipe_types.end()-1))->capacity);
	}
	// interpolation
	else
	{
		for (auto it = pipe_types.begin(); it != pipe_types.end()-1; it++)
			if ((*it)->capacity <= capacity && capacity < (*(it+1))->capacity)
				return (capacity - (*it)->capacity)*
					   ((*it+1)->price - (*it)->price)/
					   ((*it+1)->capacity-(*it)->capacity);
	}
	return 0;
}
double ALGORITHM::func_cT() const noexcept
{
	double ret = 0;
	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		if ((*it)->self_capabilities)
			continue;

		double nearest_station_distance = HUGE_VAL;
		for (auto jt = objects.begin(); jt != objects.end(); jt++)
		{
			if ((*jt)->self_need)
				continue;

			double dist = (*it)->DistanceTo(**jt);
			if (nearest_station_distance > dist)
				nearest_station_distance = dist;
		}

		ret += PipeCapacityToPrice((*it)->self_need)*nearest_station_distance;
		ret += g1 * nearest_station_distance + g2 * (*it)->self_need;
	}
	return ret;
}
double ALGORITHM::OutOfAcceptance() const noexcept
{
	double ret = 0;
	for (auto it = objects.begin(); it != objects.end(); it++)
		ret += max((*it)->total_need - (*it)->total_capabilities, 0);
	return ret;
}
bool ALGORITHM::InTabooList(const std::vector<MOVE*>& list, const MOVE& move) const noexcept
{
	for (auto it = list.begin(); it != list.end(); it++)
		if (**it == move)
			return true;
	return false;
}
void ALGORITHM::Core() noexcept
{
	progressBar.SetRange(max_iterations);
	progressBar.SetPos(0);
	progressBar.Show();

	std::fstream FS(export_path, std::fstream::out);

	if (!FS && export_path.length() > 1)
		MessageBox(0, L"B��d zapisu do pliku.", L"B��d", MB_OK);

	double cT = func_cT();

	SOLUTION s;
	SOLUTION sBest;

	RefreshTotalObjectValues(s.tab);
	double sF = SolutionCost(s.tab)+kf*cT*OutOfAcceptance();

	std::vector<MOVE*> tabooList;
	tabooList.reserve(taboo_max_size);

	for (size_t iteration = 0; iteration < max_iterations; iteration++)
	{
		MOVE bestCandidateMove;
		SOLUTION bestCandidate;
		double bestF = HUGE_VAL;

		MOVE CandidateMove;
		SOLUTION Candidate = s;

		// addition or replacement
		for (auto it = objects.begin(); it != objects.end(); it++)
			for (auto jt = objects.begin(); jt != objects.end(); jt++)
			{
				if (*it == *jt || // source and target are the same object
					(*jt)->self_capabilities) // target is water station
					continue;

				bool exists = false;
				for (auto kt = Candidate.tab.begin(); kt != Candidate.tab.end(); kt++)
					if ((*kt)->obj_source == *it && (*kt)->obj_target == *jt)
					{	
						// connection already is in solution
						for (auto mt = pipe_types.begin(); mt != pipe_types.end(); mt++)
						{
							CONNECTION con(*it, *jt, *mt);	
							
							CONNECTION* old = *kt;				// simplified replacement
							CandidateMove.Assign(old, &con);

							if (InTabooList(tabooList, CandidateMove))
								continue;
							
							*kt = &con;
							RefreshTotalObjectValues(Candidate.tab);
							double candF = SolutionCost(Candidate.tab)+kf*cT*OutOfAcceptance();
							if (bestF > candF)
							{
								bestF = candF;
								bestCandidate = Candidate;
								bestCandidateMove = CandidateMove;
							}
							*kt = old;
						}
						exists = true; 
						break;
					}

				if (!exists) // connection is not in solution
				{
					for (auto mt = pipe_types.begin(); mt != pipe_types.end(); mt++)
					{
						CONNECTION con(*it, *jt, *mt);		
						CandidateMove.Assign(MOVE_ADD, &con);

						if (InTabooList(tabooList, CandidateMove))
							continue;
							
						Candidate.tab.push_back(&con);				// addition
						RefreshTotalObjectValues(Candidate.tab);
						double candF = SolutionCost(Candidate.tab)+kf*cT*OutOfAcceptance();
						if (bestF > candF)
						{
							bestF = candF;
							bestCandidate = Candidate;
							bestCandidateMove = CandidateMove;
						}
						Candidate.tab.pop_back();

						for (auto kt = Candidate.tab.begin(); kt != Candidate.tab.end(); kt++)
						{
							CONNECTION* old = *kt;				// multiple replacement
							CandidateMove.Assign(old, &con);

							if (InTabooList(tabooList, CandidateMove))
								continue;
							
							*kt = &con;
							RefreshTotalObjectValues(Candidate.tab);
							double candF = SolutionCost(Candidate.tab)+kf*cT*OutOfAcceptance();
							if (bestF > candF)
							{
								bestF = candF;
								bestCandidate = Candidate;
								bestCandidateMove = CandidateMove;
							}
							*kt = old;
						}
					}
				}
			}
		
		for (auto kt = Candidate.tab.begin(); kt != Candidate.tab.end(); kt++)
		{
			CONNECTION* old = *kt;
			CandidateMove.Assign(MOVE_DELETE, old); // deletion

			if (InTabooList(tabooList, CandidateMove))
				continue;

			kt = Candidate.tab.erase(kt);
			RefreshTotalObjectValues(Candidate.tab);
			double candF = SolutionCost(Candidate.tab)+kf*cT*OutOfAcceptance();
			if (bestF > candF)
			{
				bestF = candF;
				bestCandidate = Candidate;
				bestCandidateMove = CandidateMove;
			}
			kt = Candidate.tab.insert(kt, old);
		}

		s = bestCandidate;
		tabooList.push_back(new MOVE(bestCandidateMove));
		if (tabooList.size() > taboo_max_size)
		{
			delete *(tabooList.begin());
			tabooList.erase(tabooList.begin());
		}

		FS << bestF << std::endl;

		if (sF > bestF)
		{
			sF = bestF;
			sBest = bestCandidate;
			best_iteration = iteration;
		}

		progressBar.SetPos(iteration);
	}

	sBest.Export(connections);
	FS.close();
	progressBar.Hide();
}

void ALGORITHM::ApplyDefaultExportPath(std::wstring& path) noexcept
{
	export_path = path;
}
void ALGORITHM::LoadFromFile(std::fstream& File)
{
	File.read(reinterpret_cast<char*>(&g1), sizeof(double));
	File.read(reinterpret_cast<char*>(&g2), sizeof(double));

	File.read(reinterpret_cast<char*>(&kf), sizeof(double));
	File.read(reinterpret_cast<char*>(&taboo_max_size), sizeof(size_t));
	File.read(reinterpret_cast<char*>(&max_iterations), sizeof(size_t));


}
void ALGORITHM::SaveToFile(std::fstream& File)
{
	File.write(reinterpret_cast<const char*>(&g1), sizeof(double));
	File.write(reinterpret_cast<const char*>(&g2), sizeof(double));

	File.write(reinterpret_cast<const char*>(&kf), sizeof(double));
	File.write(reinterpret_cast<const char*>(&taboo_max_size), sizeof(size_t));
	File.write(reinterpret_cast<const char*>(&max_iterations), sizeof(size_t));
}

