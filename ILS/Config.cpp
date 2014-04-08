#include <ctime>
#include <cstdlib>
#include <random>
#include "Config.h"
#include "Solution.h"
#include "Instance.h"


std::default_random_engine Config::generator(time(NULL));

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
	outputExt.open("logExt.txt");
}

void Config::closeLog()
{
	output.close();
	outputExt.close();
}

bool Config::comparePairs(const pair<int, int> &a, const pair<int, int> &b)
{
	return a.second < b.second;
}


void Config::initialize(Solution & s, Instance * inst)
{	
	//srand(time(NULL));
	inst->initialize();
	s.initialize(inst);
}

int Config::Rand()
{
	uniform_int_distribution<int> distribution(1, 2000202);
	return distribution(generator);
}