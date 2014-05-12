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
	double minTime = time[0];
	double maxTime = time[0];
	double sumTime = 0;
	double sumGap = 0;
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
	gap.resize(solution.size());
	for (int i = 0; i < solution.size(); i++)
	{
		gap[i] = 100 * (solution[i].bestCost - minCost) / minCost;
		sumGap += gap[i];
	}

	agap = sumGap / solution.size();
	bestTime = minTime;
	bestCost = minCost;
	worstTime = maxTime*1.0;
	worstCost = maxCost;
	medianCost = sumCost * 1.0/solution.size();
	medianTime = sumTime * 1.0/solution.size();
}