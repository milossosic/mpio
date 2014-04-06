#include "IteratedLocalSearch.h"
#include "Config.h"

IteratedLocalSearch::IteratedLocalSearch()
{
}


IteratedLocalSearch::~IteratedLocalSearch()
{
}

//uklanjanje jednog slucajnog sc-a
void IteratedLocalSearch::localSearchScRemove(Solution & s)
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
		s.genInitBsSet();
		n++;
	} while (!(cov = s.coverUsers()) && n<10);
	if (!cov)
		s = oldSolution;
}

//uklanjanje jedne slucajne bs-e i postavljanje nove
void IteratedLocalSearch::localSearchBsInvert(Solution & s)
{
	Solution oldSolution = Solution(s);

	int n = 0;
	bool cov = false;
	do
	{
		s.removeRandomBs();
		s.insertRandomBs();
		n++;
	} while (!(cov = s.coverUsers()) && n<10);
	if (!cov)
		s = oldSolution;

}

//uklanjanje jedne slucajne bs-e
void IteratedLocalSearch::localSearchBsRemove(Solution & s)
{
	if (s.bsSet.size() == s.bsFixed)
		return;
	Solution oldSolution = Solution(s);

	int n = 0;
	bool cov = false;

	s.removeRandomBs();

	while (!(cov = s.coverUsers()) && n<10)
	{
		s.insertRandomBs();
		s.removeRandomBs();
		n++;
	} 
	if (!cov)
		s = oldSolution;

}


void IteratedLocalSearch::acceptanceCriterion(Solution & s)
{
	s.currentCost = s.totalCost();
	if (s.currentCost < s.bestCost)
	{
		s.bestCost = s.currentCost;
		noImprovementCount = 0;
		bestSolution = Solution(s);
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

void IteratedLocalSearch::runILS(Solution & s)
{
	s.generateInitialSolution();


	currentIter = 0;
	while (currentIter++ < Config::MAX_ITER && noImprovementCount < 100)
	{
		//if (noImprovementCount/10 % 2)
		perturbationBsConnInvert(s);

		//	perturbationScInvert();
		//localSearchScRemove();
		//localSearchBsInvert();
		//if (noImprovementCount / 20 % 2 == 0)
		localSearchBsInvert(s);
		//else
		localSearchBsRemove(s);
		acceptanceCriterion(s);
	}
}
