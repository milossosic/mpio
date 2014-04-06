#pragma once
#include <utility>
#include <string>
#include <fstream>
#include "Instance.h"
using namespace std;

class Config
{
public:
	string input;
	ofstream output;
	const static int MAX_ITER = 1000;
	
	void openLog();
	void closeLog();
	void initialize(Instance & inst);
	void finalize();
	Config();
	~Config();
	static bool comparePairs(const pair<int, int> &a, const pair<int, int> &b);
};

