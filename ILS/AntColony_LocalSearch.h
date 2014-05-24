#pragma once
#include "AntColonyOptimization.h"
#include "Solution.h"
#include "Instance.h"
#include "IteratedLocalSearch.h"
class AntColony_LocalSearch :
	public AntColonyOptimization
{
public:
	IteratedLocalSearch *ils;

	void initialize(Solution & s, Instance * inst);
	void runAcoLs(Solution & s, Instance * inst, Config & c);
	void localSearch(Instance * inst);

	double lsPercent;

	AntColony_LocalSearch();
	~AntColony_LocalSearch();
};

