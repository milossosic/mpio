#include "AntColonyOptimization.h"
#include "Config.h"

AntColonyOptimization::AntColonyOptimization()
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

AntColonyOptimization::AntColonyOptimization(int _antCount, double _initBsPh, double _initScPh, double _e, double _d, double _p, double _a, double _b)
{
	antCount = _antCount;
	initialBsPh = _initBsPh;
	initialScPh = _initScPh;
	evaporationConst = _e;
	incPower = _p;
	devideConst = _d;
	alpha = _a;
	beta = _b;


	ants.resize(antCount);
}
AntColonyOptimization::~AntColonyOptimization()
{
}

void AntColonyOptimization::initialize(Solution & s,Instance * inst)
{
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

	//
	s.coverUserStop = inst->bsNewCount/2;
}
void AntColonyOptimization::updatePheromones(Instance * inst, int iter)
{
	//update resenja
	int oldCost = bestSolutionGlobal.currentCost;
	bestSolutionLocal = Solution(ants[0].s);
	for (int i = 1; i < antCount; i++)
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
	
	
	int id;

	//phSc
	for (int i = 0; i < phSc.size(); i++)
	{
		phSc[i] *= 1 - evaporationConst;
	}
	
	//phBs
	for (int i = 0; i < phBs.size(); i++)
	{
		phBs[i] *= 1 - evaporationConst;
	}
	if (iter % 5 == 0)
	{
		double inc = pow(devideConst / bestSolutionGlobal.currentCost, incPower);
		for (int i = 0; i < bestSolutionGlobal.bsSet.size(); i++)
		{
			id = bestSolutionGlobal.bsSet[i].first - inst->bsOldCount;
			phBs[id] += inc;
		}
		for (int i = 0; i < bestSolutionGlobal.scSet.size(); i++)
		{
			id = bestSolutionGlobal.scSet[i] - inst->scOldCount;
			phSc[id] += inc;
		}
	}
	else
	{
		double inc = pow(devideConst / bestSolutionLocal.currentCost, incPower);
		for (int i = 0; i < bestSolutionLocal.bsSet.size(); i++)
		{
			id = bestSolutionLocal.bsSet[i].first - inst->bsOldCount;
			phBs[id] += inc;
		}
		for (int i = 0; i < bestSolutionLocal.scSet.size(); i++)
		{
			id = bestSolutionLocal.scSet[i] - inst->scOldCount;
			phSc[id] += inc;
		}
	}
	

	if (bestSolutionGlobal.currentCost < oldCost)
	{
		noImprovementCount = 0;
	}
	else
	{
		noImprovementCount++;
	}
}
void AntColonyOptimization::runAnts(Instance * inst)
{
	for (int i = 0; i < antCount; i++)
	{
		ants[i].runAnt(phBs, phSc, alpha, beta, inst);
	}
}

void AntColonyOptimization::runAco(Solution & s, Instance * inst, Config & c)
{
	initialize(s, inst);

	c.noImprovement = (inst->usCount > 80 ? inst->usCount / 2 : (inst->usCount>30?30:inst->usCount));
	c.MAX_ITER = inst->usCount * 5;
	while (iter++ < c.MAX_ITER && noImprovementCount < c.noImprovement)
	{
		runAnts(inst);
		updatePheromones(inst,iter);
	}

	bestSolutionGlobal.bestCost = bestSolutionGlobal.currentCost;
}