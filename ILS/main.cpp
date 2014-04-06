#include <iostream>
#include <sstream>
#include "IteratedLocalSearch.h"
#include "Reader.h"
#include "Writer.h"
#include "Solution.h"
#include "Config.h"
#include "Instance.h"
using namespace std;

//void initialize()
//{
//	srand(time(NULL));
//
//	//inicijalizacija skupova bs i us
//	switchingCenters.clear();
//	switchingCenters.resize(scOldCount + scNewCount);
//	baseStations.clear();
//	baseStations.resize(currentBsCount);
//	users.clear();
//	users.resize(usCount);
//
//	currentSwitchingCenters.clear();
//
//	for (int i = 0; i < usCount; i++)
//	{
//		users[i].id = i;
//	}
//
//
//	for (int j = 0; j < currentBsCount; j++)
//	{
//		for (int i = 0; i < usCount; i++)
//		{
//			if (usBsRadius[i][j] < bsRadius)
//			{
//				users[i].bsSet.push_back(make_pair(j, usBsRadius[i][j]));
//			}
//		}
//	}
//
//	//sortiranje po broju baznih stanica
//	for (int i = 0; i < usCount; i++)
//	{
//		sort(users[i].bsSet.begin(), users[i].bsSet.end(), comparePairs);
//	}
//
//	//init kapaciteta sc-a
//	for (int i = 0; i < scOldCount + scNewCount; i++)
//	{
//		switchingCenters[i].capacity = scCapacity;
//	}
//
//	//ispis DEBUG
//	//printCurrentBs();
//	originalBaseStations.clear();
//	originalBaseStations = deque<baseStation>(baseStations);
//
//	//punjenje skupa bs na raspolaganju
//	resetCurrentBS();
//	//stare veze bs - sc
//	for (int i = 0; i < bsOldCount; i++)
//	{
//		for (int j = 0; j < scOldCount; j++)
//		{
//			if (bsScConnMatrix[i][j] == 1)
//			{
//				originalBaseStations[i].scId = j;
//			}
//		}
//	}
//	solution.bsFixed = -1;
//	solution.bsSet.clear();
//	solution.scSet.clear();
//	//DEBUG
//	//printCurrentBs();
//
//}

int main()
{
	Config conf;
	Reader reader;
	Writer writer;
	IteratedLocalSearch ils;

	conf.openLog();
	
	for (int i = 1; i <= 1; i++)
	{
		Solution sol;
		ostringstream ostr;
		ostr << "instance/instSmall/instSmall" << i << ".txt";
		conf.input = ostr.str();

		reader.readInput(conf, sol.inst);
		conf.initialize(sol.inst);

		
		ils.runILS(sol);

		writer.printResult(conf, sol);
	}

	conf.closeLog();

	return 0;
}