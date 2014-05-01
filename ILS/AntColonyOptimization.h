#pragma once
#include "Solution.h"
#include "Instance.h"
#include "Ant.h"
class AntColonyOptimization
{
public:
	int antCount;
	double initialBsPh;
	double initialScPh;
	double evaporationConst;
	double devideConst;
	double alpha, beta;
	vector<Ant> ants;
	//Ant bestAnt;

	Solution bestSolutionGlobal;
	Solution bestSolutionLocal;
	vector<double> phBs;
	vector<double> phSc;


	void updatePheromones();

	
	
	void initialize(Solution & s, Instance * inst);

	void runAnts(Instance * inst);

	void runAco(Solution & s, Instance * inst);

	AntColonyOptimization();
	AntColonyOptimization(int _antCount, double a, double b);
	~AntColonyOptimization();
};

