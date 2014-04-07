#include "Results.h"


Results::Results()
{
}


Results::~Results()
{
}

void Results::calculate()
{
	if (solution.size() < 1)
		return;
	int minCost = solution[0].bestCost;
	int maxCost = solution[0].bestCost;
	double sumCost = 0;
	float minTime = time[0];
	float maxTime = time[0];
	double sumTime = 0;
	for (int i = 0; i < solution.size(); i++)
	{
		if (solution[i].bestCost < minCost)
			minCost = solution[i].bestCost;
		if (solution[i].bestCost > maxCost)
			maxCost = solution[i].bestCost;
		if (time[i] < minTime)
			minTime = time[i];
		if (time[i] > maxTime)
			maxTime = time[i];
		sumTime += time[i];
		sumCost += solution[i].bestCost;
	}

	bestTime = minTime;
	bestCost = minCost;
	worstTime = maxTime*1.0;
	worstCost = maxCost*1.0;
	medianCost = sumCost * 1.0/solution.size();
	medianTime = sumTime * 1.0/solution.size();
}