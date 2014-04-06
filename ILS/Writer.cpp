#include <iostream>
#include "Writer.h"
#include "Solution.h"
#include "Config.h"
using namespace std;
Writer::Writer()
{
}


Writer::~Writer()
{
}

void Writer::printResult(Config & c, Solution & s)
{
	//cout << "total cost = " << totalCost() << ";\tminCost = " << previousCost << ";\titer = " << currentIter << ";\tnoImprovement = " << noImprovementCount << endl;
	cout << "sc: ";
	for (int i = 0; i < s.scSet.size(); i++)
	{
		cout << s.scSet[i] << " ";
	}
	cout << endl;
	cout << "bs: ";
	for (int i = 0; i < s.bsSet.size(); i++)
	{
		cout << s.bsSet[i].first << "->" << s.bsSet[i].second << " ";
	}
	cout << endl << s.bestCost << endl;
	c.output << s.bestCost << endl;
}