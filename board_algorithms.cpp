#include <string>
#include <fstream>

#include "solution.h"
#include "move.h"
#include "board.h"

void BOARD::RefreshTotalObjectValues(const std::vector<CONNECTION*>& tab) noexcept
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
bool BOARD::IsSolutionAcceptable() const noexcept
{
	for (auto it = objects.begin(); it != objects.end(); it++)
		if (!(*it)->Ok())
			return false;

	return true;
}
double BOARD::SolutionCost(const std::vector<CONNECTION*>& tab) const noexcept
{
	double ret = 0;

	for (auto it = tab.begin(); it != tab.end(); it++)
		ret += (*it)->Cost(g1, g2);

	return ret;
}
void BOARD::ClearConnections() noexcept
{
	for (auto it = connections.begin(); it != connections.end(); it++)
		delete *it;

	connections.clear();
}

double BOARD::PipeCapacityToPrice(double capacity) const noexcept
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
			if ((*it)->capacity <= capacity && capacity < (*it+1)->capacity)
				return (capacity - (*it)->capacity)*
					   ((*it+1)->price - (*it)->price)/
					   ((*it+1)->capacity-(*it)->capacity);
	}
	return 0;
}
double BOARD::func_cT() const noexcept
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
double BOARD::OutOfAcceptance() const noexcept
{
	double ret = 0;
	for (auto it = objects.begin(); it != objects.end(); it++)
		ret += max((*it)->total_need - (*it)->total_capabilities, 0);
	return ret;
}
bool BOARD::InTabooList(const std::vector<MOVE*>& list, const MOVE& move) const noexcept
{
	for (auto it = list.begin(); it != list.end(); it++)
		if (**it == move)
			return true;
	return false;
}
void BOARD::TabooAlgorithmCore() noexcept
{
	progressBar.SetRange(max_iterations);
	progressBar.SetPos(0);
	progressBar.Show();

	std::fstream FS("out.txt", std::fstream::out);

	if (!FS)
		MessageBox(0, L"file error", L"error", MB_OK);

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
		}

		progressBar.SetPos(iteration);
	}

	sBest.Export(connections);
	FS.close();
	progressBar.Hide();
	RedrawWindow(target->GetHwnd(), 0, 0, RDW_INTERNALPAINT);
}