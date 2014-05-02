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
	bool localSearchScInvert(Solution & s, Instance * inst);
	bool localSearchScInvertConn(Solution & s, Instance * inst);
	void localSearchBsInvertNew(Solution & s, Instance * inst);

	bool localSearchBsRemove(Solution & s, Instance * inst);

	void localSearchBsAdd(Solution & s, Instance * inst);

	void perturbationScInvertNew(Solution & s, Instance * inst);

	void perturbationNew(Solution & s,Instance * inst);
	void localSearchNew(Solution & s, Instance * inst);
	void localSearchAco(Solution & s, Instance * inst);
	void acceptanceCriterionNew(Solution & s, Instance * inst, Config & c);
	void runILSNew(Solution & s, Instance * i, Config & c);
};

