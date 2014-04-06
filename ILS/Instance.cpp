#include "Instance.h"
#include "Config.h"

Instance::Instance()
{
}


Instance::~Instance()
{
}

void Instance::initialize()
{
	
	for (int i = 0; i < usCount; i++)
	{
		for (int j = 0; j < bsOldCount + bsNewCount; j++)
		{
			if (usBsRadius[i][j] < bsRadius)
			{
				users[i].bsSet.push_back(make_pair(j, usBsRadius[i][j]));
			}
		}
	}

	for (int i = 0; i < usCount; i++)
	{
		sort(users[i].bsSet.begin(), users[i].bsSet.end(), Config::comparePairs);
	}


}
