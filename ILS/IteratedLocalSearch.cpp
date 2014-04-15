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
		if (n % 4 == 0)
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
		if (n % 4 == 0)
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
bool IteratedLocalSearch::localSearchBsRemove(Solution & s, Instance * inst)
{
	if (s.bsSet.size() == s.bsFixed)
		return false;
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
	{
		s = *oldSolution;
		return false;
	}

	return true;
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

void IteratedLocalSearch::runILSNew(Solution & s, Instance * inst, Config & c)
{
	//
	//s.generateInitialSolutionGreedy(inst);
	s.generateInitialSolutionRandom(inst);

	currentIter = 0;
	while (currentIter++ < 1000/*Config::MAX_ITER*/ && noImprovementCount < 150)
	{

		perturbation(s);

		localSearchNew(s, inst, c);

		acceptanceCriterion(s, inst, c);
	}
	c.outputExt << currentIter << " " << noImprovementCount << endl;
	cout << currentIter << " " << noImprovementCount << endl << endl;


}

void IteratedLocalSearch::runILS(Solution & s, Instance * inst, Config & c)
{
	s.generateInitialSolutionRandom(inst);
	
	//s.greedyConn = true;
	currentIter = 0;
	while (currentIter++ < 1000/*Config::MAX_ITER*/ && noImprovementCount < 150)
	{

		perturbation(s);
		
		localSearch(s,inst,c);
		
		acceptanceCriterion(s, inst, c);
	}
	c.outputExt << currentIter << " " << noImprovementCount << endl;
	cout << currentIter << " " << noImprovementCount << endl;
}

void IteratedLocalSearch::acceptanceCriterion(Solution & s, Instance * inst, Config & c)
{
	s.currentCost = s.totalCost(inst);
	if (s.currentCost < s.bestCost)
	{
		solIter.push_back(make_pair(s.currentCost, currentIter));
		c.outputExt << s.currentCost << " " << currentIter << endl;
		cout << s.currentCost << " " << currentIter << endl;
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
	if(!s.greedyConn )
		perturbationBsConnInvert(s);
	if (currentIter % 3 == 1)
		perturbationScInvert(s);
}

void IteratedLocalSearch::localSearchNew(Solution & s, Instance * inst, Config & c)
{
	bool better = false;
	int i = 0, rand;
	if (localSearchBsRemove(s, inst))
		return;
	while (i < s.bsSet.size() && !better)
	{
		rand = Config::Rand() % s.currentBaseStations.size();
		for (int j = 0; j < inst->scOldCount; j++)
		{
			if (inst->bsScConnCost[s.currentBaseStations[rand]][j] < inst->bsScConnCost[s.bsSet[i].first][j])
			{
				better = true;
				int temp = s.bsSet[i].first;
				s.bsSet[i].first = s.currentBaseStations[rand];
				s.currentBaseStations[rand] = temp;
				s.bsSet[i].second = j;
			}
		}
		for (int j = 0; j < s.scSet.size(); j++)
		{
			if (inst->bsScConnCost[s.currentBaseStations[rand]][s.scSet[j]] < inst->bsScConnCost[s.bsSet[i].first][s.scSet[j]])
			{
				better = true;
				int temp = s.bsSet[i].first;
				s.bsSet[i].first = s.currentBaseStations[rand];
				s.currentBaseStations[rand] = temp;
				s.bsSet[i].second = s.scSet[j];
			}
		}
		if (s.coverUsers(inst))
			return;
		i++;
	}
	//moze da se doda da odmah izracuna i koliko je bolje ovo resenje od prethodnog

}

void IteratedLocalSearch::localSearch(Solution & s, Instance * inst, Config & c)
{
	int r = currentIter % 10;
	
	
	//localSearchScAdd(s, inst);
	if (inst->usCount < (inst->scOldCount + s.scSet.size())*inst->scCapacity - inst->scCapacity)
		localSearchScRemove(s, inst);
	
	if (r == 8)
	{
		localSearchBsAdd(s, inst);
		
		//if (noImprovementCount > 100)
		//{
		//	//localSearchBsAdd(s, inst);
		//	//noImprovementCount = 50;
		//}
	}
	if (r <8 && r >1 )
	//if (r<7 || r>16)
		localSearchBsRemove(s, inst);

	
	if (noImprovementCount>35 )
		localSearchBsInvert(s, inst);

	
}