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
	int noImprovementCount = 0;
	vector<pair<int, int>> solIter;
	Solution bestSolution;

	IteratedLocalSearch();
	~IteratedLocalSearch();

	bool localSearchScRemove(Solution & s, Instance * inst);
	void localSearchScAdd(Solution & s, Instance * inst);
	void localSearchBsInvert(Solution & s, Instance * inst);
	bool localSearchBsRemove(Solution & s, Instance * inst);
	void localSearchBsAdd(Solution & s, Instance * inst);
	void perturbationBsConnInvert(Solution & s);
	void perturbationScInvert(Solution & s);
	void perturbationScInvertNew(Solution & s);
	void perturbation(Solution & s);
	void perturbationNew(Solution & s,Instance * inst);
	void localSearch(Solution & s, Instance * inst, Config & c);
	void localSearchNew(Solution & s, Instance * inst, Config & c);
	void acceptanceCriterion(Solution & s, Instance * inst, Config & c);
	void runILS(Solution & s, Instance * i, Config & c);
	void runILSNew(Solution & s, Instance * i, Config & c);
};

