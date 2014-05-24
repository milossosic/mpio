#pragma once
#include <vector>
#include "Solution.h"
using namespace std;
class Results
{
public:
	double medianCost;
	double medianTime;
	int bestCost;
	double bestTime;
	int worstCost;
	double worstTime;
	double agap;
	double standardDeviation;
	vector<double> gap;
	vector<Solution> solution;
	vector<double> time;

	void calculate();
	Results();
	~Results();
};

