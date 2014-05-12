#include "AntColony_LocalSearch.h"
#include "Config.h"
#include "IteratedLocalSearch.h"
AntColony_LocalSearch::AntColony_LocalSearch()
{
	antCount = 8;
	initialBsPh = 50;
	initialScPh = 50;
	evaporationConst = 0.1;
	devideConst = 500000;
	incPower = 3;
	alpha = 3;
	beta = 1;

	ants.resize(antCount);
}


AntColony_LocalSearch::~AntColony_LocalSearch()
{
}


void AntColony_LocalSearch::initialize(Solution & s, Instance * inst)
{
	ils = new IteratedLocalSearch();
	iter = 0;
	noImprovementCount = 0;
	MAXnoImprovementCount = (inst->usCount > 30 ? 30 : inst->usCount);
	bestSolutionGlobal.currentCost = INT_MAX;
	for (int i = 0; i < antCount; i++)
	{
		ants[i].s = Solution(s);
		ants[i].s.generateBsMustSet(inst);
		for (int j = 0; j < ants[i].s.bsFixed; j++)
		{
			ants[i].s.setRandomScConn(j, inst);
		}
	}
	// feromoni
	phBs.resize(inst->bsNewCount);
	phSc.resize(inst->scNewCount);
	for (int i = 0; i < inst->bsNewCount; i++)
	{
		phBs[i] = initialBsPh;
	}
	for (int i = 0; i < inst->scNewCount; i++)
	{
		phSc[i] = initialScPh;
	}
}


void AntColony_LocalSearch::localSearch(Instance *inst)
{
	for (int i = 0; i < antCount; i++)
	{
		ils->localSearchNew(ants[i].s, inst);
	}
}

void AntColony_LocalSearch::runAcoLs(Solution & s, Instance * inst)
{
	initialize(s, inst);
	
	while (iter++ < Config::MAX_ITER && noImprovementCount < MAXnoImprovementCount)
	{
		runAnts(inst);
		localSearch(inst);
		updatePheromones(inst,iter);
	}

	bestSolutionGlobal.bestCost = bestSolutionGlobal.currentCost;
}