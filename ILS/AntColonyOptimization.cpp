#include "AntColonyOptimization.h"
#include "Config.h"

AntColonyOptimization::AntColonyOptimization()
{
}

AntColonyOptimization::AntColonyOptimization(int _antCount, int a, int b)
{
	antCount = _antCount;
	initialBsPh = a;
	initialScPh = b;
	ants.resize(antCount);
}
AntColonyOptimization::~AntColonyOptimization()
{
}

void AntColonyOptimization::initialize()
{
	
}

void AntColonyOptimization::runAco(Solution & s, Instance * inst)
{
	initialize();

	int iter = 0;
	while(iter < Config::MAX_ITER)
	{
		runAnts(inst);
		updatePheromones();

		iter++;
	}
}
