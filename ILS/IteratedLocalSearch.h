#pragma once
#include "Solution.h"
class IteratedLocalSearch
{
public:
	int currentIter;
	int noImprovementCount;
	Solution bestSolution;

	IteratedLocalSearch();
	~IteratedLocalSearch();

	void localSearchScRemove(Solution & s);
	void localSearchBsInvert(Solution & s);
	void localSearchBsRemove(Solution & s);
	void perturbationBsConnInvert(Solution & s);
	void perturbationScInvert(Solution & s);
	void acceptanceCriterion(Solution & s);
	void runILS(Solution & s);
};

