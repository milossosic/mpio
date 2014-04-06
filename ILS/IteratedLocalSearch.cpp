#include "IteratedLocalSearch.h"


IteratedLocalSearch::IteratedLocalSearch()
{
}


IteratedLocalSearch::~IteratedLocalSearch()
{
}

//uklanjanje jednog slucajnog sc-a
void IteratedLocalSearch::localSearchScRemove(Solution & s)
{
	if (solution.scSet.size() <= 1)
		return;
	oldSolution = Solution(solution);

	//uniform_int_distribution<int> distConn(0, solution.scSet.size() - 1);
	int randSc = rand() % solution.scSet.size();
	//int randSc = distConn(generator);
	currentSwitchingCenters.push_back(solution.scSet[randSc]);
	solution.scSet.erase(solution.scSet.begin() + randSc);
	int n = 0;
	//////////////////////////////////FIX ME
	bool cov = false;
	do
	{
		resetCapacities();
		resetCurrentBS();
		resetSolutionBs();
		generateInitialBsSolutionSet();
		n++;
	} while (!(cov = coverUsers()) && n<10);
	if (!cov)
		solution = oldSolution;
}

//uklanjanje jedne slucajne bs-e i postavljanje nove
void IteratedLocalSearch::localSearchBsInvert(Solution & s)
{
	cout << "ls bs invert: ";
	oldSolution = Solution(solution);

	int n = 0;
	//////////////////////////////////FIX ME
	bool cov = false;
	do
	{

		int x = solution.bsSet.size() - solution.bsFixed;
		randBs = rand() % x + solution.bsFixed;
		//randBs = solution.bsSet[randBs].first;
		cout << solution.bsSet[randBs].first << " ";

		solution.removeBs(randBs);

		randBs = rand() % currentBaseStations.size();
		randBs = currentBaseStations[randBs];
		randConnSc = rand() % (scOldCount + solution.scSet.size());

		cout << randBs << endl;

		if (randConnSc >= scOldCount)
		{
			randConnSc -= scOldCount;
			randConnSc = solution.scSet[randConnSc];
		}

		solution.insertBs(randBs, randConnSc);

		n++;
	} while (!(cov = coverUsers()) && n<10);
	if (!cov)
		solution = oldSolution;

}

//uklanjanje jedne slucajne bs-e
void IteratedLocalSearch::localSearchBsRemove(Solution & s)
{
	//if (solution.bsSet.size() == solution.bsFixed)
	return;
	oldSolution = Solution(solution);

	int n = 0;
	//////////////////////////////////FIX ME
	bool cov = false;
	do
	{
		//uklanjanje jedne slucajne bs-e i postavljanje nove
		int x = solution.bsSet.size() - solution.bsFixed;
		if (x == 0)
			return;
		int randBs = rand() % x + solution.bsFixed;

		solution.removeBs(randBs);

		n++;
	} while (!(cov = coverUsers()) && n<10);
	if (!cov)
		solution = oldSolution;

}


void IteratedLocalSearch::acceptanceCriterion(Solution & s)
{
	currentCost = totalCost();
	if (currentCost < previousCost)
	{
		previousCost = currentCost;
		noImprovementCount = 0;
		bestSolution = Solution(solution);
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
	//uniform_int_distribution<int> distConn(0, solution.bsSet.size() - 1);
	int x = solution.bsSet.size() - solution.bsFixed;
	int randBs1 = rand() % x + solution.bsFixed;

	//int randBs1 = rand() % solution.bsSet.size();
	//int randBs1 = distConn(generator);
	int randBs2 = rand() % x + solution.bsFixed;
	//int randBs2 = distConn(generator);
	while (randBs1 == randBs2)
	{
		randBs2 = rand() % x + solution.bsFixed;
	}
	int temp = solution.bsSet[randBs1].first;
	solution.bsSet[randBs1].first = solution.bsSet[randBs2].first;
	solution.bsSet[randBs2].first = temp;
}

//sc zamena
void IteratedLocalSearch::perturbationScInvert(Solution & s)
{
	//oldSolution = Solution(solution);
	//uklanjanje jednog slucajnog sc-a
	int randSc1 = rand() % (solution.scSet.size() - scOldCount);
	solution.removeSc(randSc1);
	//i vracanje sc-a
	int id;
	int randSc2 = rand() % currentSwitchingCenters.size();
	id = currentSwitchingCenters[randSc2];
	solution.insertSc(randSc2);

	for (int i = 0; i < solution.bsSet.size(); i++)
	{
		if (solution.bsSet[i].second == randSc1)
			solution.bsSet[i].second = id;
	}

}

void IteratedLocalSearch::runILS()
{
	generateInitialSolution();


	currentIter = 0;
	while (currentIter++ < MAX_ITER && noImprovementCount < 100)
	{
		//if (noImprovementCount/10 % 2)
		perturbationBsConnInvert();

		//	perturbationScInvert();
		//localSearchScRemove();
		//localSearchBsInvert();
		//if (noImprovementCount / 20 % 2 == 0)
		localSearchBsInvert();
		//else
		localSearchBsRemove();
		acceptanceCriterion();
	}
}
