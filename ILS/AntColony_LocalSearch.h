#pragma once
#include "AntColonyOptimization.h"
#include "Solution.h"
#include "Instance.h"
class AntColony_LocalSearch :
	public AntColonyOptimization
{
public:

	void runAcoLs(Solution & s, Instance * inst);

	AntColony_LocalSearch();
	~AntColony_LocalSearch();
};

