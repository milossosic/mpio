#pragma once
#include <utility>
#include <vector>
#include "Solution.h"
#include "Instance.h"
#include "Config.h"
#include "CplexSolver.h"
class IteratedLocalSearch
{
public:
	int currentIter;
	int noImprovementCount = 0;
	vector<pair<int, int>> solIter;
	Solution bestSolution;
	CplexSolver *cplexSolver;
	IteratedLocalSearch();
	~IteratedLocalSearch();

	bool localSearchScRemove(Solution & s, Instance * inst);
	bool localSearchScRemoveCplex(Solution & s, Instance * inst, CplexSolver * c);
	void localSearchScAdd(Solution & s, Instance * inst);
	bool localSearchScInvert(Solution & s, Instance * inst);
	bool localSearchScInvertConn(Solution & s, Instance * inst);
	void localSearchBsInvert(Solution & s, Instance * inst);
	void localSearchBsInvertNew(Solution & s, Instance * inst);
	void localSearchBsInvertCplex(Solution & s, Instance * inst, CplexSolver * c);
	void perturbationBsInvertCplex(Solution & s, Instance * inst, CplexSolver * c);

	bool localSearchBsRemoveLS(Solution & s, Instance * inst);
	bool localSearchBsRemove(Solution & s, Instance * inst);
	bool localSearchBsRemoveCplex(Solution & s, Instance * inst,CplexSolver *cpl);

	void localSearchBsAdd(Solution & s, Instance * inst);
	void localSearchBsAddCplex(Solution & s, Instance * inst);

	void perturbationBsConnInvert(Solution & s);
	void perturbationScInvert(Solution & s);
	void perturbationScInvertNew(Solution & s, Instance * inst);
	void perturbationScInvertCplex(Solution & s, Instance * inst);

	void perturbation(Solution & s);
	void perturbationNew(Solution & s,Instance * inst);
	void perturbationCplex(Solution & s, Instance * inst, CplexSolver *cpl);

	void localSearch(Solution & s, Instance * inst, Config & c);
	void localSearchNew(Solution & s, Instance * inst, Config & c);
	void localSearchCplex(Solution & s, Instance * inst, Config & c, CplexSolver *cpl);
	void acceptanceCriterion(Solution & s, Instance * inst, Config & c);
	void acceptanceCriterionNew(Solution & s, Instance * inst, Config & c);
	void acceptanceCriterionCplex(Solution & s, Instance * inst, Config & c, CplexSolver * cpl);

	void runILS(Solution & s, Instance * i, Config & c);
	void runILSNew(Solution & s, Instance * i, Config & c);
	void runILSCplex(Solution & s, Instance * i, Config & c);
};

