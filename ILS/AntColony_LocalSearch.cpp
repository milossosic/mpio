#include "AntColony_LocalSearch.h"
#include "Config.h"
#include "IteratedLocalSearch.h"
#include <cmath>
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

	lsPercent = 0.2;

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
	vector<pair<int,int>> tAnts;
	for (int i = 0; i < antCount; i++)
	{
		tAnts.push_back(make_pair(i,ants[i].s.bestCost));
	}
	sort(tAnts.begin(), tAnts.end(), Config::comparePairsAsc);

	int lsAntCount = ceil(lsPercent * antCount);

	for (int i = 0; i < lsAntCount; i++)
	{
		ils->localSearchNew(ants[tAnts[i].first].s, inst);
	}
	
	
	for (int i = 0; i < lsAntCount; i++)
	{
		int rand = Config::Rand() % (antCount - lsAntCount) + lsAntCount;
		ils->localSearchNew(ants[tAnts[rand].first].s, inst);
	}
}

void AntColony_LocalSearch::runAcoLs(Solution & s, Instance * inst, Config & c)
{
	initialize(s, inst);
	
	c.noImprovement = (inst->usCount > 80 ? inst->usCount / 2 : 40);
	c.MAX_ITER = inst->usCount * 5;
	while (iter++ < c.MAX_ITER && noImprovementCount < c.noImprovement)
	{
		runAnts(inst);
		localSearch(inst);
		updatePheromones(inst,iter);
	}

	bestSolutionGlobal.bestCost = bestSolutionGlobal.currentCost;
}