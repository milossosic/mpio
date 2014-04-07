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
	do
	{
		s.genInitBsSet(inst);
		n++;
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
	do
	{
		s.genInitBsSet(inst);
		n++;
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
		s.insertRandomBs(inst);
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
		s.insertRandomBs(inst);
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
	s.insertRandomBs(inst);

	while (!(cov = s.coverUsers(inst)) && n<10)
	{
		n++;
	}
	if (!cov)
		s = *oldSolution;
}

void IteratedLocalSearch::acceptanceCriterion(Solution & s, Instance * inst)
{
	s.currentCost = s.totalCost(inst);
	if (s.currentCost < s.bestCost)
	{
		s.bestCost = s.currentCost;
		noImprovementCount = 0;
		bestSolution = *(new Solution(s));
		//cout << "bolje "<< endl;
	}
	else
	{
		noImprovementCount++;
	}
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

void IteratedLocalSearch::runILS(Solution & s, Instance * inst)
{
	s.generateInitialSolution(inst);
	int r;
	
	currentIter = 0;
	while (currentIter++ < 300/*Config::MAX_ITER*/ && noImprovementCount < 50)
	{

		//if (noImprovementCount/10 % 2)
		perturbationBsConnInvert(s);

		perturbationScInvert(s);
		r = Config::Rand() % 10;
		//if (r < 8)
			localSearchScRemove(s, inst);
		//else
			//localSearchScAdd(s, inst);
		//localSearchBsInvert();
		//if (noImprovementCount / 20 % 2 == 0)

			localSearchBsInvert(s, inst);

		//if (noImprovementCount > 10)
		r = Config::Rand() % 10;
		if (r>8)
			localSearchBsAdd(s, inst);
		else
			localSearchBsRemove(s, inst);

		//for (int i = 0; i < 5; i++)
		//{
		//	localSearchBsInvert(s, inst);
		//	s.currentCost = s.totalCost(inst);
		//	if (s.currentCost < s.bestCost)
		//	{
		//		s.bestCost = s.currentCost;
		//		noImprovementCount = 0;
		//		bestSolution = *(new Solution(s));
		//		//cout << "bolje "<< endl;
		//	}
		//}
		acceptanceCriterion(s, inst);
	}
}
