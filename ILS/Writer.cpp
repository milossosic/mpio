#include <iostream>
#include "Writer.h"
#include "Results.h"
#include "Config.h"
#include "IteratedLocalSearch.h"
using namespace std;
Writer::Writer()
{
}


Writer::~Writer()
{
}

void Writer::printResult(Config & c, Results & r)
{
	//cout << "total cost = " << totalCost() << ";\tminCost = " << previousCost << ";\titer = " << currentIter << ";\tnoImprovement = " << noImprovementCount << endl;
	/*cout << "sc: ";
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
	cout << endl << s.bestCost << endl;*/
	for (int i = 0; i < r.solution.size(); i++)
	{
		c.output << r.solution[i].bestCost << " " << r.time[i] << endl;
	}
	//c.output << r..bestCost << " " << time << endl << endl;
}

void Writer::printExtended(Config & c, Results & r)
{
	c.output << r.medianCost << " " << r.bestCost << " " << r.worstCost << " ";
	c.output << r.medianTime << " " << r.bestTime << " " << r.worstTime << endl;
}

void Writer::printExt(Config & c, IteratedLocalSearch * ils)
{
	//c.outputExt << 
}