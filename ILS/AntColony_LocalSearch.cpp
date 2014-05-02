#include "AntColony_LocalSearch.h"
#include "Config.h"
#include "IteratedLocalSearch.h"
AntColony_LocalSearch::AntColony_LocalSearch()
{
}


AntColony_LocalSearch::~AntColony_LocalSearch()
{
}

void AntColony_LocalSearch::runAcoLs(Solution & s, Instance * inst)
{
	initialize(s, inst);
	IteratedLocalSearch *ils = new IteratedLocalSearch();
	iter = 0;
	noImprovementCount = 0;
	while (iter < Config::MAX_ITER && noImprovementCount < inst->usCount)
	{
		runAnts(inst);
		for (int i = 0; i < antCount; i++)
		{
			ils->localSearchNew(ants[i].s, inst);
		}
		updatePheromones(inst);

		iter++;
	}

	bestSolutionGlobal.bestCost = bestSolutionGlobal.currentCost;
}