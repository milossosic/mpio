#pragma once
#include "Solution.h"
class Ant
{
public:
	Solution s;


	void runAnt(vector<double> &phBs, vector<double> &phSc, double alpha, double beta, Instance * inst);

	Ant();
	~Ant();
};

