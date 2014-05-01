#pragma once
#include "Solution.h"
#include "Instance.h"
#include "Ant.h"
class AntColonyOptimization
{
public:
	int antCount;
	int initialBsPh;
	int initialScPh;
	vector<Ant> ants;
	Ant bestAnt;
	Solution bestSolution;
	vector<double> phBs;
	vector<double> phSc;


	void updatePheromones();

	
	
	void initialize();

	void runAnts(Instance * inst);

	void runAco(Solution & s, Instance * inst);

	AntColonyOptimization();
	AntColonyOptimization(int _antCount, int a, int b);
	~AntColonyOptimization();
};

