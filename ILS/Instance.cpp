#include "Instance.h"
#include "Config.h"
#include "Solution.h"

Instance::Instance()
{
}

Instance::Instance(Instance & i)
{
	usCount = i.usCount;
	bsOldCount = i.bsOldCount;
	bsNewCount = i.bsNewCount;
	scOldCount = i.scOldCount;
	scNewCount = i.scNewCount;
	scCapacity = i.scCapacity;
	bsCapacity = i.bsCapacity;
	scCost = i.scCost;
	bsCost = i.bsCost;
	bsRadius = i.bsRadius;
	/*int usBsRadius[1000][100];
	int bsScConnCost[100][100];
	int bsScConnMatrix[1000][100];

	deque<user> users;*/
}

Instance::~Instance()
{
}

void Instance::initialize()
{
	
	

	for (int i = 0; i < usCount; i++)
	{
		sort(users[i].bsSet.begin(), users[i].bsSet.end(), Config::comparePairsAsc);
	}


}
