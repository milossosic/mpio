#include "Config.h"
#include "Solution.h"
#include "Instance.h"
Config::Config()
{
	//output.open("log.txt");
}


Config::~Config()
{
}

void Config::openLog()
{
	output.open("log.txt");
}

void Config::closeLog()
{
	output.close();
}

bool Config::comparePairs(const pair<int, int> &a, const pair<int, int> &b)
{
	return a.second < b.second;
}


void Config::initialize(Solution & s, Instance * inst)
{	
	inst->initialize();
	s.initialize(inst);
}
