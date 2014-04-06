#include "Config.h"
#include "Instance.h"

Config::Config()
{
	output.open("log.txt");
}


Config::~Config()
{
}

bool Config::comparePairs(const pair<int, int> &a, const pair<int, int> &b)
{
	return a.second < b.second;
}

void Config::initialize(Instance & inst)
{
	output.open("log.txt");
	inst.initialize();
}

void Config::finalize()
{
	output.close();
}