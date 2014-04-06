#pragma once
#include <utility>
#include <string>
#include <fstream>
using namespace std;

static class Config
{
public:
	static string input;
	static ofstream output;
	const int MAX_ITER = 1000;
	

	static void initialize(Instance & inst);
	static void finalize();
	Config();
	~Config();
	static bool comparePairs(const pair<int, int> &a, const pair<int, int> &b);
};

