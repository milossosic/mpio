#include <iostream>
#include <utility>
#include <deque>
#include "IteratedLocalSearch.h"
#include "Config.h"
#include "Solution.h"
#include "Instance.h"
#include "CplexSolver.h"
IteratedLocalSearch::IteratedLocalSearch()
{
}
IteratedLocalSearch::~IteratedLocalSearch()
{
}

bool IteratedLocalSearch::localSearchScRemove(Solution & s, Instance * inst)
{
	if (s.scSet.size() < 1)
		return false;
	

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
void IteratedLocalSearch::localSearchBsInvert(Solution & s, Instance * inst)
{
	

	int n = 0, tempBs;
	bool cov = false;
	if (s.bsFixed == s.bsSet.size())
		return;
	do
	{
		tempBs = s.removeRandomBs();
		s.insertRandomBs(inst, s.greedyConn);
		s.setRandomScConn(s.bsSet.size() - 1, inst, s.greedyConn);
		n++;
	} while (!(cov = s.coverUsers(inst)) && n<10);
	
		

}
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
void IteratedLocalSearch::perturbationBsConnInvert(Solution & s)
{
	s.bsIdInvert();
}
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




void IteratedLocalSearch::acceptanceCriterion(Solution & s, Instance * inst, Config & c)
{
	s.currentCost = s.totalCost(inst);
	if (s.currentCost < s.bestCost)
	{
		//solIter.push_back(make_pair(s.currentCost, currentIter));
		//c.outputExt << s.currentCost << " " << currentIter << endl;
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
	if (r <8 && r >1)
		//if (r<7 || r>16)
		localSearchBsRemove(s, inst);


	if (noImprovementCount>35)
		localSearchBsInvert(s, inst);


}
void IteratedLocalSearch::runILS(Solution & s, Instance * inst, Config & c)
{
	s.generateInitialSolutionRandom(inst);

	//s.greedyConn = true;
	currentIter = 0;
	while (currentIter++ < 1000/*Config::MAX_ITER*/ && noImprovementCount < 150)
	{

		perturbation(s);

		localSearch(s, inst, c);

		acceptanceCriterion(s, inst, c);
	}
	c.outputExt << currentIter << " " << noImprovementCount << endl;
	cout << currentIter << " " << noImprovementCount << endl;
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
	if (localSearchScRemove(s, inst))
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
			if (newCost < oldCost)
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
void IteratedLocalSearch::runILSNew(Solution & s, Instance * inst, Config & c)
{
	//s.generateInitialSolutionGreedy(inst);
	s.generateInitialSolutionRandom(inst);

	currentIter = 0;
	while (currentIter++ < 1000/*Config::MAX_ITER*/ && noImprovementCount < 30)
	{

		perturbationNew(s, inst);

		localSearchNew(s, inst, c);

		acceptanceCriterion(s, inst, c);
	}
	c.outputExt << currentIter << " " << noImprovementCount << endl;
	//cout << currentIter << " " << noImprovementCount << endl << endl;


}





void IteratedLocalSearch::localSearchBsInvertCplex(Solution & s, Instance * inst, CplexSolver * cpl)
{
	int tempBs, in, out, cost;
	if (s.bsFixed == s.bsSet.size())
		return;

	s.minLsCost = s.bestCost;
	
	for (int i = s.bsFixed; i < s.bsSet.size(); i++)
	{
		tempBs = s.removeBsCplex(s.bsFixed, inst);

		int rand = Config::Rand() % (s.currentBaseStations.size());
		s.insertBsCplex(rand, inst);
		if (cpl->solve(s.B, s.M) && (cost = s.totalCostCplex(inst, cpl)) < s.minLsCost)
		{
			s.minLsCost = cost;
			out = s.currentBaseStations[tempBs - 1];
			in = s.bsSet[s.bsSet.size() - 1].first;
		}
		s.removeBsCplex(s.bsSet.size() - 1, inst);

		/*for (int j = 0; j < s.currentBaseStations.size() - 1; j++)
		{
			s.insertBsCplex(0, inst);
			if (cpl->solve(s.B, s.M) && (cost = s.totalCostCplex(inst, cpl)) < s.minLsCost)
			{
				s.minLsCost = cost;
				out = s.currentBaseStations[tempBs-1-j];
				in = s.bsSet[s.bsSet.size() - 1].first;
			}
			s.removeBsCplex(s.bsSet.size()-1,inst);
		}*/
		s.insertBsCplex(0, inst);
	}

	if (s.minLsCost < s.bestCost)
	{
		s.removeRealBsCplex(out, inst);
		s.insertRealBsCplex(in, inst);
	}
}
bool IteratedLocalSearch::localSearchScRemoveCplex(Solution & s, Instance * inst, CplexSolver * cpl)
{
	if (s.scSet.size() < 1 || (inst->usCount > (inst->scOldCount + s.scSet.size())*inst->scCapacity - inst->scCapacity))
		return false;


	s.M[s.removeRandomSc() - inst->scOldCount] = 0;

	//int n = 0;
	////////////////////////////////////FIX ME
	//bool cov = false;
	//int bsN = (inst->bsNewCount - s.bsFixed) / 2;
	//do
	//{
	//	s.genInitBsSetCplex(inst, bsN);
	//	n++;
	//	if (n % 4 == 0)
	//		bsN++;
	//} while (!(cov = cpl->solve(s.B, s.M)) && n<3);
	//if (!cov)
	//{
	//	s.insertSc(s.currentSwitchingCenters.size() - 1);
	//	s.M[s.scSet[s.scSet.size() - 1] - inst->scOldCount] = 1;
	//}
	return true;
}
bool IteratedLocalSearch::localSearchBsRemoveCplex(Solution & s, Instance * inst, CplexSolver *cpl)
{
	if (s.bsSet.size() == s.bsFixed)
		return false;
	bool cov = false;
	int tempBs, out, cost;

	s.minLsCost = s.bestCost;
	for (int i = s.bsFixed; i < s.bsSet.size(); i++)
	{
		tempBs = s.removeBsCplex(s.bsFixed, inst);
		if (cpl->solve(s.B, s.M) && (cost = s.totalCostCplex(inst, cpl)) < s.minLsCost)
		{
			s.minLsCost = cost;
			out = s.currentBaseStations[tempBs];

		}
		s.insertBsCplex(tempBs, inst);
	}



	if (s.minLsCost < s.bestCost)
	{
		s.removeRealBsCplex(out, inst);
		return true;
	}

	return false;
}
void IteratedLocalSearch::localSearchBsAddCplex(Solution & s, Instance * inst)
{
	if (s.currentBaseStations.size() == 0)
		return;
	s.insertRandomBs(inst, s.greedyConn);
	s.B[s.bsSet[s.bsSet.size() - 1].first - inst->bsOldCount] = 1;
}
void IteratedLocalSearch::perturbationScInvertCplex(Solution & s, Instance * inst)
{
	if (s.scSet.size() == 0)
		return;
	int id1 = s.removeRandomSc();
	s.M[id1 - inst->scOldCount] = 0;
	//i vracanje slucajnog sc-a
	int id2 = s.insertRandomSc();
	s.M[id2 - inst->scOldCount] = 1;
}
void IteratedLocalSearch::perturbationBsInvertCplex(Solution & s, Instance * inst, CplexSolver * cpl)
{
	if (s.bsFixed == s.bsSet.size())
		return;
	int rand = Config::Rand() % (s.bsSet.size());
	s.removeBsCplex(rand, inst);
	rand = Config::Rand() % (s.currentBaseStations.size() - 1);
	s.insertBsCplex(rand, inst);
}

void IteratedLocalSearch::perturbationCplex(Solution & s, Instance * inst, CplexSolver *cpl)
{
	//perturbationBsConnInvert(s);
	
	perturbationBsInvertCplex(s, inst, cpl);
	perturbationBsInvertCplex(s, inst, cpl);
	
	
	perturbationScInvertCplex(s, inst);
		//localSearchBsAdd(s, inst);
	

	if (noImprovementCount > 5)
		localSearchBsAddCplex(s, inst);
}
void IteratedLocalSearch::localSearchCplex(Solution & s, Instance * inst, Config & c, CplexSolver *cpl)
{
	localSearchScRemoveCplex(s, inst, cpl);

	//if(noImprovementCount>5)
		if(localSearchBsRemoveCplex(s, inst, cpl))
			return;

	localSearchBsInvertCplex(s, inst, cpl);
}
void IteratedLocalSearch::acceptanceCriterionCplex(Solution & s, Instance * inst, Config & c, CplexSolver * cpl)
{
	s.currentCost = s.minLsCost;
	if (s.currentCost < s.bestCost)
	{
		//solIter.push_back(make_pair(s.currentCost, currentIter));
		//c.outputExt << s.currentCost << " " << currentIter << endl;
		cout << s.currentCost << " " << currentIter << endl;
		s.bestCost = s.currentCost;
		noImprovementCount = 0;
		bestSolution.bestCost = s.bestCost;

	}
	else
	{
		noImprovementCount++;
	}
}
void IteratedLocalSearch::runILSCplex(Solution & s, Instance * inst, Config & c)
{
	cplexSolver = new CplexSolver();
	cplexSolver->initialize(inst);

	s.B.resize(inst->bsNewCount);
	s.M.resize(inst->scNewCount);

	s.generateInitialSolutionCplex(inst, cplexSolver);

	cout << s.bestCost << endl;

	currentIter = 0;
	while (currentIter++ < Config::MAX_ITER && noImprovementCount < 30)
	{
		cout << "*** ";
		perturbationCplex(s, inst, cplexSolver);

		localSearchCplex(s, inst, c, cplexSolver);

		acceptanceCriterionCplex(s, inst, c, cplexSolver);
	}


	//c.outputExt << currentIter << " " << noImprovementCount << endl;
	//cout << currentIter << " " << noImprovementCount << endl << endl;


}