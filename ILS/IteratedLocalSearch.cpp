#include <iostream>
#include "IteratedLocalSearch.h"
#include "Config.h"
#include "Solution.h"
#include "Instance.h"
IteratedLocalSearch::IteratedLocalSearch()
{
}


IteratedLocalSearch::~IteratedLocalSearch()
{
}

//uklanjanje jednog slucajnog sc-a
void IteratedLocalSearch::localSearchScRemove(Solution & s, Instance * inst)
{
	if (s.scSet.size() < 1)
		return;
	Solution oldSolution = Solution(s);

	s.removeRandomSc();

	int n = 0;
	//////////////////////////////////FIX ME
	bool cov = false;
	int bsN = ((inst->bsNewCount - s.bsFixed) / 2 > 5) ? 5 : (inst->bsNewCount - s.bsFixed) / 2;
	do
	{
		s.genInitBsSet(inst,bsN);
		n++;
		if (n % 2 == 0)
			bsN++;
	} while (!(cov = s.coverUsers(inst)) && n<10);
	if (!cov)
		s = oldSolution;
}

void IteratedLocalSearch::localSearchScAdd(Solution & s, Instance * inst)
{
	if (s.currentSwitchingCenters.size() < 1)
		return;
	Solution oldSolution = Solution(s);

	s.insertRandomSc();

	int n = 0;
	//////////////////////////////////FIX ME
	bool cov = false;
	int bsN = ((inst->bsNewCount - s.bsFixed) / 2 > 5) ? 5 : (inst->bsNewCount - s.bsFixed) / 2;
	do
	{
		
		s.genInitBsSet(inst,bsN);
		n++;
		if (n % 2 == 0)
			bsN++;
	} while (!(cov = s.coverUsers(inst)) && n<10);
	if (!cov)
		s = oldSolution;
}

//uklanjanje jedne slucajne bs-e i postavljanje nove
void IteratedLocalSearch::localSearchBsInvert(Solution & s, Instance * inst)
{
	Solution *oldSolution =new  Solution(s);

	int n = 0;
	bool cov = false;
	if (s.bsFixed == s.bsSet.size())
		return;
	do
	{
		s.removeRandomBs();
		s.insertRandomBs(inst,s.greedyConn);
		n++;
	} while (!(cov = s.coverUsers(inst)) && n<10);
	if (!cov)
		s = *oldSolution;

}

//uklanjanje jedne slucajne bs-e
void IteratedLocalSearch::localSearchBsRemove(Solution & s, Instance * inst)
{
	if (s.bsSet.size() == s.bsFixed)
		return;
	Solution *oldSolution = new Solution(s);

	int n = 0;
	bool cov = false;
	s.removeRandomBs();

	while (!(cov = s.coverUsers(inst)) && n<10)
	{
		s.insertRandomBs(inst,s.greedyConn);
		s.removeRandomBs();
		n++;
	} 
	if (!cov)
		s = *oldSolution;

}

void IteratedLocalSearch::localSearchBsAdd(Solution & s, Instance * inst)
{
	if (s.currentBaseStations.size() == 0)
		return;
	Solution *oldSolution = new Solution(s);

	int n = 0;
	bool cov = false;
	s.insertRandomBs(inst,s.greedyConn);

	while (!(cov = s.coverUsers(inst)) && n<10)
	{
		n++;
	}
	if (!cov)
		s = *oldSolution;
}


//bs zamena
void IteratedLocalSearch::perturbationBsConnInvert(Solution & s)
{
	s.bsIdInvert();
}

//sc zamena
void IteratedLocalSearch::perturbationScInvert(Solution & s)
{
	if (s.scSet.size() == 0)
		return;
	int id1 = s.removeRandomSc();
	//i vracanje slucajnog sc-a
	int id2 = s.insertRandomSc();

	for (int i = 0; i < s.bsSet.size(); i++)
	{
		if (s.bsSet[i].second == id1)
			s.bsSet[i].second = id2;
	}

}

void IteratedLocalSearch::runILS(Solution & s, Instance * inst, Config & c)
{
	s.generateInitialSolution(inst);
	
	//s.greedyConn = true;
	currentIter = 0;
	while (currentIter++ < Config::MAX_ITER && noImprovementCount < 100)
	{

		perturbation(s);
		
		localSearch(s,inst,c);
		
		acceptanceCriterion(s, inst, c);
	}
	c.outputExt << currentIter << " " << noImprovementCount << endl;
}

void IteratedLocalSearch::acceptanceCriterion(Solution & s, Instance * inst, Config & c)
{
	s.currentCost = s.totalCost(inst);
	if (s.currentCost < s.bestCost)
	{
		solIter.push_back(make_pair(s.currentCost, currentIter));
		c.outputExt << s.currentCost << " " << currentIter << endl;
		s.bestCost = s.currentCost;
		noImprovementCount = 0;
		bestSolution = *(new Solution(s));

	}
	else
	{
		noImprovementCount++;
	}
}

void IteratedLocalSearch::perturbation(Solution & s)
{
	if(!s.greedyConn)
		perturbationBsConnInvert(s);
	if (currentIter % 2 == 1)
		perturbationScInvert(s);
}

void IteratedLocalSearch::localSearch(Solution & s, Instance * inst, Config & c)
{
	int r = Config::Rand() % 10;
	//if (r < 8)
	if (currentIter % 70 == 19)
		localSearchScAdd(s, inst);
	else
		localSearchScRemove(s, inst);
	
	if (currentIter % 20 == 9)
		localSearchBsAdd(s, inst);
	else
		localSearchBsRemove(s, inst);

	localSearchBsInvert(s, inst);

	//for (int i = 0; i < 3; i++)
	//{
	//	localSearchBsInvert(s, inst);
	//	s.currentCost = s.totalCost(inst);
	//	if (s.currentCost < s.bestCost)
	//	{
	//		s.bestCost = s.currentCost;
	//		noImprovementCount = 0;
	//		bestSolution = *(new Solution(s));
	//		c.outputExt << s.currentCost << " " << currentIter << endl;
	//		//cout << "bolje "<< endl;
	//	}
	//}
}