#pragma once
#include <utility>
#include <string>
#include <fstream>
#include <random>
#include <vector>
#include "Solution.h"
#include "Instance.h"
using namespace std;

class Config
{
public:
	string input;
	ofstream output;
	ofstream outputExt;

	

	const static int MAX_ITER = 1000;
	int noImprovement = 400;
	const static int lsDepth = 5;

	static default_random_engine generator;
	

	void openLog();
	void closeLog();
	void initialize(Solution & s,Instance *i);
	void finalize();
	static int Rand();
	static vector<string> dirList(string dir, string name);
	Config();
	~Config();
	static bool comparePairsAsc(const pair<int, int> &a, const pair<int, int> &b);
	static bool comparePairsDsc(const pair<int, int> &a, const pair<int, int> &b);
};

