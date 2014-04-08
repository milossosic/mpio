#pragma once
#include <utility>
#include <vector>
#include "Solution.h"
#include "Instance.h"
#include "Config.h"
class IteratedLocalSearch
{
public:
	int currentIter;
	int noImprovementCount;
	vector<pair<int, int>> solIter;
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
	void perturbation(Solution & s);
	void localSearch(Solution & s, Instance * inst, Config & c);
	void acceptanceCriterion(Solution & s, Instance * inst, Config & c);
	void runILS(Solution & s, Instance * i, Config & c);
};

