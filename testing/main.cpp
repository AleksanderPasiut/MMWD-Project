#include <iostream>
#include <vector>
#include <forward_list>
#include <cmath>
#include <time.h>
#include <fstream>

using namespace std;

// DEFINICJA FUNKCJI CELU
struct point { double x, y; };
bool operator== (const point& pt1, const point& pt2) { return pt1.x == pt2.x && pt1.y == pt2.y; }
double f(point arg)
{
	return sqrt(arg.x*arg.x + arg.y*arg.y) / 2 + 10 * sin(arg.x)*sin(arg.y);
}

// ALGORYTM BRUTE-FORCE
// wyszukiwanie minimum z podan¹ dok³adnoœci¹ w podanym zakresie
point brute_force()
{
	point ret;
	double fitness = HUGE_VAL;

	for (double x = -10; x <= 10; x += 0.01)
		for (double y = -10; y <= 10; y += 0.01)
			if (fitness > f({x,y}))
			{
				fitness = f({x,y});
				ret = {x,y};
			}

	return ret;
}

// ALGORYTM TABOO-SEARCH
// wyszukiwanie o podanej dok³adnoœci, rozwi¹zaniu pocz¹tkowym, liczbie iteracji i maksymalnym rozmiarze listy taboo
point taboo_search(point s0, size_t limit, size_t taboo_size)
{
	fstream file("out.txt", fstream::out);

	point s = s0;
	point sBest = s;
	double sF = f(s);

	vector<point> taboo;

	for (size_t i = 0; i < limit; i++ )
	{
		point bestCandidate;
		double bestF = HUGE_VAL;

		if (s.x < -10 || s.x > 10 || s.y < -10 || s.y > 10)
		{
			s = sBest;
			bestF = f(s);
		}
		else
		{
			for (double x = -0.4; x <= 0.4; x += 0.05)
				for (double y = -0.4; y <= 0.4; y += 0.05)
				{
					point candidate = {s.x+x, s.y+y};
					bool in_taboo = false;
					for (auto it = taboo.begin(); it != taboo.end(); it++)
						if (abs((*it).x - candidate.x) <= 0.005 && abs((*it).y - candidate.y) <= 0.005)
						{
							in_taboo = true;
							break;
						}

					if (!in_taboo)
					{
						double candF = f(candidate);
						if (bestF > candF)
						{
							bestF = candF;
							bestCandidate = candidate;
						}
					}
				}

			if (bestF == HUGE_VAL)
				break;

			s = bestCandidate;

			taboo.push_back(s);
			if (taboo.size() > taboo_size)
				taboo.erase(taboo.begin());

		}

		file << s.x << " " << s.y << " " << f(s);

		if (sF > bestF)
		{
			sF = bestF;
			sBest = bestCandidate;
			file << " 1";
		}
		
		file << endl;
	}

	file.close();
	return sBest;
}

int main()
{
	/*point solution;
	solution = brute_force();
	cout << "brute force solution: " << endl;
	cout << "f(" << solution.x << " " << solution.y << ") = " << f(solution) << endl;

	point s0 = { 5, 5 };
	solution = taboo_search(s0, 10000, 5000);
	cout << "taboo search solution: " << endl;
	cout << "initial solution: " << s0.x << " " << s0.y << endl;
	cout << "f(" << solution.x << " " << solution.y << ") = " << f(solution) << endl;
	*/

	cin.get();
	cin.ignore();
	return 0;
 }