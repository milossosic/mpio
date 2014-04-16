#include <iostream>
#include <utility>
#include <deque>
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
bool IteratedLocalSearch::localSearchScRemove(Solution & s, Instance * inst)
{
	if (s.scSet.size() < 1)
		return false;
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
	{
		s.insertSc(s.currentSwitchingCenters.size()-1);
	}
	return true;
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
	Solution oldSolution = Solution(s);

	int n = 0, tempBs;
	bool cov = false;
	if (s.bsFixed == s.bsSet.size())
		return;
	do
	{
		tempBs = s.removeRandomBs();
		//s.insertRandomBs(inst,s.greedyConn);
		s.insertBs(tempBs, -1);
		s.setRandomScConn(s.bsSet.size() - 1, inst, s.greedyConn);
		n++;
	} while (!(cov = s.coverUsers(inst)) && n<10);
	//if (!cov)
	//{
	//	//s = oldSolution;

	//}
		

}

//uklanjanje jedne slucajne bs-e
bool IteratedLocalSearch::localSearchBsRemove(Solution & s, Instance * inst)
{
	if (s.bsSet.size() == s.bsFixed)
		return false;
	//Solution oldSolution = Solution(s);
	//deque<pair<int, int>> tempBsSet = deque<pair<int, int>>(s.bsSet);
	//deque<int> tempCurrentBs = deque<int>(s.currentBaseStations);
	int n = 0;
	bool cov = false;
	int tempBs = s.removeRandomBs();

	while (!(cov = s.coverUsers(inst)) && n<10)
	{
		s.insertBs(tempBs,-1);
		s.setRandomScConn(s.bsSet.size()-1,inst,s.greedyConn);
		//s.insertRandomBs(inst,s.greedyConn);
		tempBs = s.removeRandomBs();
		n++;
	} 
	if (!cov)
	{
		s.insertBs(tempBs, -1);
		s.setRandomScConn(s.bsSet.size() - 1, inst, s.greedyConn);
		//s.bsSet.clear();
		//s.bsSet = deque<pair<int, int>>(tempBsSet);
		//s.currentBaseStations.clear();
		//s.currentBaseStations = deque<int>(tempCurrentBs);
		//s = oldSolution;
		return false;
	}

	return true;
}

void IteratedLocalSearch::localSearchBsAdd(Solution & s, Instance * inst)
{
	if (s.currentBaseStations.size() == 0)
		return;
	s.insertRandomBs(inst,s.greedyConn);
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

void IteratedLocalSearch::perturbationScInvertNew(Solution & s)
{

}

void IteratedLocalSearch::runILSNew(Solution & s, Instance * inst, Config & c)
{
	//s.generateInitialSolutionGreedy(inst);
	s.generateInitialSolutionRandom(inst);

	currentIter = 0;
	while (currentIter++ < 1000/*Config::MAX_ITER*/ && noImprovementCount < 30)
	{

		perturbationNew(s,inst);

		localSearchNew(s, inst, c);

		acceptanceCriterion(s, inst, c);
	}
	c.outputExt << currentIter << " " << noImprovementCount << endl;
	//cout << currentIter << " " << noImprovementCount << endl << endl;


}

void IteratedLocalSearch::runILSCplex(Solution & s, Instance * inst, Config & c)
{
	cplexSolver = new CplexSolver();
	cplexSolver->initialize(inst);
	//s.generateInitialSolutionGreedy(inst);
	s.generateInitialSolutionRandom(inst);

	currentIter = 0;
	while (currentIter++ < Config::MAX_ITER && noImprovementCount < 30)
	{
		perturbationNew(s, inst);

		localSearchNew(s, inst, c);

		acceptanceCriterion(s, inst, c);
	}
	//c.outputExt << currentIter << " " << noImprovementCount << endl;
	//cout << currentIter << " " << noImprovementCount << endl << endl;


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
		//treba samo bestCost
		bestSolution.bestCost = s.bestCost;
		//bestSolution.bsSet = deque<pair<int, int>>(s.bsSet);
		//bestSolution.scSet = deque<int>(s.scSet);
		//bestSolution = Solution(s);

	}
	else
	{
		noImprovementCount++;
	}
}

void IteratedLocalSearch::perturbation(Solution & s)
{
	if (!s.greedyConn)
	{
		perturbationBsConnInvert(s);
		
	}
	if (currentIter % 3 == 1)
		perturbationScInvert(s);
}

void IteratedLocalSearch::perturbationNew(Solution & s, Instance * inst)
{
	if (!s.greedyConn)
	{
		//perturbationBsConnInvert(s);
		localSearchBsInvert(s, inst);
		localSearchBsInvert(s, inst);
		
	}
	if (currentIter % 5 == 1)
	{
		perturbationScInvert(s);
		//localSearchBsAdd(s, inst);
	}
}

void IteratedLocalSearch::localSearchNew(Solution & s, Instance * inst, Config & c)
{
	bool better = false;
	int oldCost, newCost;
	int i, rand;
	if (inst->usCount <= (inst->scOldCount + s.scSet.size())*inst->scCapacity - inst->scCapacity)
		if(localSearchScRemove(s, inst))
			return;
	if (localSearchBsRemove(s, inst))
		return;
	//cout << " ";
	for (i = 0; i < s.bsFixed; i++)
	{
		//rand = Config::Rand() % s.currentBaseStations.size();
		for (int j = 0; j < inst->scOldCount; j++)
		{
			//cout << "o";
			oldCost = inst->bsScConnCost[s.bsSet[i].first - inst->bsOldCount][s.bsSet[i].second];
			newCost = inst->bsScConnCost[s.bsSet[i].first - inst->bsOldCount][j];
			if (newCost < oldCost)
			{
				better = true;
				s.bsSet[i].second = j;
				return;
			}
		}
		if (!better)
			for (int j = 0; j < s.scSet.size(); j++)
			{
				oldCost = inst->bsScConnCost[s.bsSet[i].first - inst->bsOldCount][s.bsSet[i].second];
				newCost = inst->bsScConnCost[s.bsSet[i].first - inst->bsOldCount][s.scSet[j]];
				if (newCost < oldCost)
				{
					better = true;
					s.bsSet[i].second = s.scSet[j];
					return;
				}
			}
	}
	
	while (i < s.bsSet.size() && !better)
	{
		//cout << "w";
		
		rand = Config::Rand() % s.currentBaseStations.size();
		for (int j = 0; j < inst->scOldCount; j++)
		{
			//cout << "o";
			oldCost = inst->bsScConnCost[s.bsSet[i].first - inst->bsOldCount][s.bsSet[i].second];
			newCost = inst->bsScConnCost[s.currentBaseStations[rand] - inst->bsOldCount][j];
			if ( newCost < oldCost )
			{
				better = true;
				int tempBs = s.bsSet[i].first;
				int tempSc = s.bsSet[i].second;
				s.originalBaseStations[s.bsSet[i].first].scId = -1;
				s.bsSet[i].first = s.currentBaseStations[rand];
				s.currentBaseStations[rand] = tempBs;
				s.bsSet[i].second = j;

				if (s.coverUsers(inst))
					return;
				else
				{
					s.bsSet[i].first = tempBs;
					s.bsSet[i].second = tempSc;
				}
				
			}
		}
		if (!better)
			for (int j = 0; j < s.scSet.size(); j++)
			{
				oldCost = inst->bsScConnCost[s.bsSet[i].first - inst->bsOldCount][s.bsSet[i].second];
				newCost = inst->bsScConnCost[s.currentBaseStations[rand] - inst->bsOldCount][s.scSet[j]];
				if (newCost < oldCost)
				{
					better = true;
					int tempBs = s.bsSet[i].first;
					int tempSc = s.bsSet[i].second;
					s.originalBaseStations[s.bsSet[i].first].scId = -1;
					s.bsSet[i].first = s.currentBaseStations[rand];
					s.currentBaseStations[rand] = tempBs;
					s.bsSet[i].second = s.scSet[j];

					if (s.coverUsers(inst))
						return;
					else
					{
						s.bsSet[i].first = tempBs;
						s.bsSet[i].second = tempSc;
					}
				}
			}
		//if (better && s.coverUsers(inst))
		//{
		//	return;
		//	//cout << endl;
		//}
		i++;
		
	}
	//cout << "! " << endl;
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

void IteratedLocalSearch::localSearchCplex(Solution & s, Instance * inst, Config & c)
{
	
}