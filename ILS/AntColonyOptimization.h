#pragma once
#include "Solution.h"
#include "Instance.h"
#include "Ant.h"
#include "Config.h"
class AntColonyOptimization
{
public:
	int iter;
	int noImprovementCount;
	int MAXnoImprovementCount;

	int antCount;
	double initialBsPh;
	double initialScPh;
	double evaporationConst;
	double devideConst;
	double alpha, beta;
	double incPower;
	vector<Ant> ants;
	//Ant bestAnt;

	Solution bestSolutionGlobal;
	Solution bestSolutionLocal;
	vector<double> phBs;
	vector<double> phSc;


	void updatePheromones(Instance * inst, int iter);

	
	
	void initialize(Solution & s, Instance * inst);

	void runAnts(Instance * inst);

	void runAco(Solution & s, Instance * inst, Config & c);

	AntColonyOptimization();
	AntColonyOptimization(int _antCount, double _initBsPh, double _initScPh, double _e, double _d, double _p, double _a, double _b);
	~AntColonyOptimization();
};

