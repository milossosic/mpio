#pragma once
#include "Solution.h"
#include "Instance.h"
class IteratedLocalSearch
{
public:
	int currentIter;
	int noImprovementCount;
	Solution bestSolution;

	IteratedLocalSearch();
	~IteratedLocalSearch();

	void localSearchScRemove(Solution & s, Instance * inst);
	void localSearchScAdd(Solution & s, Instance * inst);
	void localSearchBsInvert(Solution & s, Instance * inst);
	void localSearchBsRemove(Solution & s, Instance * inst);
	void localSearchBsAdd(Solution & s, Instance * inst);
	void perturbationBsConnInvert(Solution & s);
	void perturbationScInvert(Solution & s);
	void acceptanceCriterion(Solution & s, Instance * inst);
	void runILS(Solution & s, Instance * i);
};

