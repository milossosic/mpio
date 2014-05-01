#include "AntColonyOptimization.h"
#include "Config.h"

AntColonyOptimization::AntColonyOptimization()
{
	antCount = 20;
	initialBsPh = 1000;
	initialScPh = 1000;
	evaporationConst = 0.05;
	devideConst = 500000;
	
	alpha = 1;
	beta = 1;

	ants.resize(antCount);
}

AntColonyOptimization::AntColonyOptimization(int _antCount, double a, double b)
{
	antCount = _antCount;
	initialBsPh = a;
	initialScPh = b;
	evaporationConst = 0.4;
	devideConst = 500000;
	ants.resize(antCount);
}
AntColonyOptimization::~AntColonyOptimization()
{
}

void AntColonyOptimization::initialize(Solution & s,Instance * inst)
{
	
	for (int i = 0; i < antCount; i++)
	{
		ants[i].s = Solution(s);
		ants[i].s.generateBsMustSet(inst);
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
void AntColonyOptimization::updatePheromones()
{
	//update resenja
	bestSolutionLocal = Solution(ants[0].s);
	for (int i = 0; i < antCount; i++)
	{
		if (ants[i].s.currentCost < bestSolutionGlobal.currentCost)
		{
			bestSolutionGlobal = Solution(ants[i].s);
		}
		if (ants[i].s.currentCost < bestSolutionLocal.currentCost)
		{
			bestSolutionLocal = Solution(ants[i].s);
		}
	}
	if (bestSolutionGlobal.currentCost == 0)
		return;
	
	double inc = devideConst / bestSolutionLocal.currentCost;
	int id;

	//phSc
	for (int i = 0; i < phSc.size(); i++)
	{
		phSc[i] *= 1 - evaporationConst;
	}
	for (int i = 0; i < bestSolutionLocal.scSet.size(); i++)
	{
		id = bestSolutionLocal.scSet[i];
		phSc[id] += inc;
	}
	
	//phBs
	for (int i = 0; i < phBs.size(); i++)
	{
		phBs[i] *= 1 - evaporationConst;
	}
	for (int i = 0; i < bestSolutionLocal.bsSet.size(); i++)
	{
		id = bestSolutionLocal.bsSet[i].first;
		phBs[id] += inc;
	}
}
void AntColonyOptimization::runAnts(Instance * inst)
{
	for (int i = 0; i < antCount; i++)
	{
		ants[i].runAnt(phBs, phSc, alpha, beta, inst);
	}
}

void AntColonyOptimization::runAco(Solution & s, Instance * inst)
{
	initialize(s, inst);

	int iter = 0;
	while(iter < Config::MAX_ITER)
	{
		runAnts(inst);
		updatePheromones();

		iter++;
	}
}
