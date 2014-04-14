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
	s.originalBaseStations.resize(inst->bsOldCount + inst->bsNewCount);
	inst->users.resize(inst->usCount);

	for (int i = 0; i < inst->usCount; i++)
	{
		for (int j = 0; j < inst->bsOldCount + inst->bsNewCount; j++)
		{
			if (inst->usBsRadius[i][j] < inst->bsRadius)
			{
				inst->users[i].bsSet.push_back(make_pair(j, inst->usBsRadius[i][j]));
				s.originalBaseStations[j].users.push_back(i);
			}
		}
	}
	inst->initialize();
	s.initialize(inst);
}

int Config::Rand()
{
	uniform_int_distribution<int> distribution(1, 2000202);
	return distribution(generator);
}