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
	vector<Solution> solution;
	vector<float> time;

	void calculate();
	Results();
	~Results();
};

