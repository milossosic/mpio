#include <iostream>
#include "Writer.h"
#include "Results.h"
#include "Config.h"
#include "IteratedLocalSearch.h"
#include <iomanip>
#include <iomanip>
using namespace std;
Writer::Writer()
{
}


Writer::~Writer()
{
}

void Writer::printResult(Config & c, Results & r)
{
	int i = r.solution.size() - 1;
	Solution & s = r.solution[i];
	cout << setw(8) << s.bestCost << " ";
	cout << std::setprecision(5) << r.time[r.time.size() - 1] << endl;
	/*c.outputExt << "bs: ";
	for (int j = 0; j < s.bsSet.size(); j++)
	{
		c.outputExt << s.bsSet[j].first << "->" << s.bsSet[j].second << " ";
	}
	c.outputExt << endl << "sc: ";
	for (int j = 0; j < s.scSet.size(); j++)
	{
		c.outputExt << s.scSet[j] << " ";
	}*/
}

void Writer::printExtended(Config & c, Results & r)
{
	c.outputExt << r.bestCost << " ";
	c.outputExt << std::setprecision(5) << r.medianTime << " ";
	c.outputExt << std::setprecision(5) << r.agap << " ";
	c.outputExt << std::setprecision(5) << r.standardDeviation << endl;
}

void Writer::printExt(Config & c, IteratedLocalSearch * ils)
{
	//c.outputExt << 
}