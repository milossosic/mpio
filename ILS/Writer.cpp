#include <iostream>
#include "Writer.h"
#include "Results.h"
#include "Config.h"
#include "IteratedLocalSearch.h"
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
	c.outputExt << s.bestCost << " " << r.time[r.time.size() - 1] << endl;
	c.outputExt << "bs: ";
	for (int j = 0; j < s.bsSet.size(); j++)
	{
		c.outputExt << s.bsSet[j].first << "->" << s.bsSet[j].second << " ";
	}
	c.outputExt << endl << "sc: ";
	for (int j = 0; j < s.scSet.size(); j++)
	{
		c.outputExt << s.scSet[j] << " ";
	}
	c.outputExt << endl;
}

void Writer::printExtended(Config & c, Results & r)
{
	c.output << r.bestCost << " " << r.bestTime << endl;
	//c.outputExt << r.medianCost << " " << r.bestCost << " " << r.worstCost << " ";
	c.outputExt << r.bestCost << " " << std::setprecision(4) << r.agap << " ";
	c.outputExt << std::setprecision(4) << r.medianTime << endl;;// << " " << r.bestTime << " " << r.worstTime << endl;
}

void Writer::printExt(Config & c, IteratedLocalSearch * ils)
{
	//c.outputExt << 
}