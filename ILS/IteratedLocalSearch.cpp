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
	} while (!(cov = s.coverUsersNew(inst)) && n<10);
	if (!cov)
		s = oldSolution;
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
		{
			s.bsSet[i].second = id2;
		}
	}

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




void IteratedLocalSearch::localSearchBsInvert(Solution & s, Instance * inst)
{
	
	if (s.bsFixed == s.bsSet.size())
		return;
	int cost, tempBs;
	
	tempBs = s.removeRandomBs();

	s.insertRandomBs(inst, s.greedyConn);
	s.setRandomScConn(s.bsSet.size() - 1, inst, s.greedyConn);
		
	if (s.coverUsersNew(inst))
	{

	}


}
void IteratedLocalSearch::localSearchBsInvertNew(Solution & s, Instance * inst)
{

	if (s.bsFixed == s.bsSet.size())
		return;
	

	int rand2,rand1 = Config::Rand() % (s.bsSet.size() - s.bsFixed) + s.bsFixed;
	int scId = (s.bsSet[rand1].second + 1)%(inst->scOldCount + s.scSet.size());
	if (scId > inst->scOldCount) scId = s.scSet[scId - inst->scOldCount];
	rand2 = Config::Rand() % s.currentBaseStations.size();
	s.removeBs(rand1);

	s.insertBs(rand2,-1);
	s.setRandomScConn(s.bsSet.size() - 1, inst, s.greedyConn);
	
	if (!s.coverUsersNew(inst))
	{
		s.removeBs(s.bsSet.size()-1);
		s.insertBs(s.currentBaseStations.size()-2,scId);
	}

}
bool IteratedLocalSearch::localSearchBsRemove(Solution & s, Instance * inst)
{
	if (s.bsSet.size() == s.bsFixed)
		return false;

	int tempBs, cost, bsId;
	bsId = s.bsSet[s.bsFixed].first - inst->bsOldCount;

	for (int i = s.bsFixed; i < s.bsSet.size(); i++)
	{
		//cost = -inst->bsCost - inst->bsScConnCost[bsId][s.bsSet[s.bsFixed].second];
		tempBs = s.removeBs(s.bsFixed);


		if (s.coverUsersNew(inst))
		{
			//s.currentCost += cost;
			return true;
		}

		s.insertBs(tempBs, -1);
		s.setRandomScConn(s.bsSet.size() - 1, inst, s.greedyConn);
	}

	return false;
}
bool IteratedLocalSearch::localSearchBsRemoveLS(Solution & s, Instance * inst)
{
	if (s.bsSet.size() == s.bsFixed)
		return false;

	int tempBs;
	//bsId = s.bsSet[s.bsFixed].first - inst->bsOldCount;

	for (int i = s.bsFixed; i < s.bsSet.size(); i++)
	{
		//cost = -inst->bsCost - inst->bsScConnCost[bsId][s.bsSet[s.bsFixed].second];
		tempBs = s.removeBs(s.bsFixed);
		if (s.coverUsersNew(inst))
		{
			return true;
		}
		s.insertBs(tempBs, -1);
		s.setRandomScConn(s.bsSet.size() - 1, inst, s.greedyConn);
	}

	return false;
}
bool IteratedLocalSearch::localSearchScRemove(Solution & s, Instance * inst)
{
	if (s.scSet.size() < 1)
		return false;
	int cost, scId;
	
	Solution *sol = new Solution(s);
	
	s.removeRandomSc();
	
	
	s.genInitBsSet(inst, (inst->bsNewCount - s.bsFixed) / 2);

	if (!s.coverUsersNew(inst))
	{
		s = *sol;
		return false;
	}

	return true;
}
bool IteratedLocalSearch::localSearchScInvert(Solution & s, Instance * inst)
{
	bool notInBs = true;
	int scOld, scNew, oldCost, newCost;
	for (int k = 0; k < s.scSet.size(); k++)
	{
		scOld = s.scSet[k];
		for (int j = 0; j < s.currentSwitchingCenters.size(); j++)
		{
			scNew = s.currentSwitchingCenters[j];
			oldCost = s.totalCost(inst);
			for (int i = 0; i < s.bsSet.size(); i++)
			{
				if (s.bsSet[i].second == scOld)
				{
					notInBs = false;
					s.bsSet[i].second = scNew;
				}
			}
			if (notInBs)
				return false;
			newCost = s.totalCost(inst);
			if (newCost > oldCost)
			{
				for (int i = 0; i < s.bsSet.size(); i++)
				{
					if (s.bsSet[i].second == scNew)
						s.bsSet[i].second = scOld;
				}
			}
			else
			{
				s.scSet.erase(s.scSet.begin() + k);
				s.scSet.push_back(scNew);
				s.currentSwitchingCenters.erase(s.currentSwitchingCenters.begin() + j);
				s.currentBaseStations.push_back(scOld);
				return true;
			}
		}
	}
	return false;
}
void IteratedLocalSearch::perturbationScInvertNew(Solution & s, Instance * inst)
{
	if (s.scSet.size() == 0)
		return;
	int id1 = s.removeRandomSc();
	//i vracanje slucajnog sc-a
	int bsId;
	int id2 = s.insertRandomSc();

	for (int i = 0; i < s.bsSet.size(); i++)
	{
		bsId = s.bsSet[i].first - inst->bsOldCount;
		if (s.bsSet[i].second == id1)
		{
			s.bsSet[i].second = id2;
			s.currentCost += -inst->bsScConnCost[bsId][id1] + inst->bsScConnCost[bsId][id2];
		}
	}
}


void IteratedLocalSearch::acceptanceCriterionNew(Solution & s, Instance * inst, Config & c)
{
	s.currentCost = s.totalCost(inst);
	if (s.currentCost < s.bestCost )
	{
		if (s.coverUsersNew(inst) )
		{

			noImprovementCount = 0;
			s.bestCost = s.currentCost;
			bestSolution = Solution(s);
		}
				
	}
	else
	{
		noImprovementCount++;
	}
}
void IteratedLocalSearch::perturbationNew(Solution & s, Instance * inst)
{
	
	/*if (noImprovementCount > inst->usCount/2 && noImprovementCount%10 == 0)
	{
		localSearchBsAdd(s, inst);
	}*/
	if (noImprovementCount > 5 && currentIter % 5 == 1)
	{
		localSearchBsInvertNew(s, inst);
		localSearchBsInvertNew(s, inst);		
	}
	if (currentIter%10 == 9)
		perturbationScInvertNew(s, inst);
	
}
void IteratedLocalSearch::localSearchNew(Solution & s, Instance * inst, Config & c)
{
	bool better = false;
	int oldCost, newCost;
	int i, rand;
	if (inst->usCount <= (inst->scOldCount + s.scSet.size())*inst->scCapacity - inst->scCapacity)
	{
		if (localSearchScRemove(s, inst))
			return;
	}
	if (localSearchBsRemove(s, inst))
		return;
	
	if (localSearchScInvert(s,inst))
	{
		return;
	}
	for (i = 0; i < s.bsFixed; i++)
	{
		for (int j = 0; j < inst->scOldCount; j++)
		{
			oldCost = inst->bsScConnCost[s.bsSet[i].first - inst->bsOldCount][s.bsSet[i].second];
			newCost = inst->bsScConnCost[s.bsSet[i].first - inst->bsOldCount][j];
			if (newCost < oldCost)
			{

				s.bsSet[i].second = j;
				//s.originalBaseStations[s.bsSet[i].first].scId = j;
				//s.currentCost += newCost - oldCost;
				localSearchBsRemove(s, inst);
				return;
			}
		}
		for (int j = 0; j < s.scSet.size(); j++)
		{
			oldCost = inst->bsScConnCost[s.bsSet[i].first - inst->bsOldCount][s.bsSet[i].second];
			newCost = inst->bsScConnCost[s.bsSet[i].first - inst->bsOldCount][s.scSet[j]];
			if (newCost < oldCost)
			{
				s.bsSet[i].second = s.scSet[j];
				s.currentCost += newCost - oldCost;
				return;
			}
		}
	}

	while (i < s.bsSet.size())
	{
		int offset = 0;
		rand = Config::Rand() % s.currentBaseStations.size();
		for (int i = 0; i < Config::lsDepth; i++)
		{
			rand = (i + rand) % s.currentBaseStations.size();
			
			for (int j = 0; j < inst->scOldCount + s.scSet.size(); j++)
			{
				int scId = j < inst->scOldCount ? j : s.scSet[j - inst->scOldCount];
				oldCost = inst->bsScConnCost[s.bsSet[s.bsFixed + offset].first - inst->bsOldCount][s.bsSet[s.bsFixed + offset].second];
				newCost = inst->bsScConnCost[s.currentBaseStations[rand] - inst->bsOldCount][scId];
				if (newCost < oldCost)
				{
					//offset--;
					int tempSc = s.bsSet[s.bsFixed + offset].second;
					s.removeBs(s.bsFixed + offset);
					s.insertBs(rand, scId);

					if (s.coverUsersNew(inst))
					{
						//s.currentCost += newCost - oldCost;
						localSearchBsRemove(s, inst);
						return;
					}
					else
					{
						s.removeBs(s.bsSet.size() - 1);
						s.insertBs(s.currentBaseStations.size() - 2, tempSc);
					}
				}
			}
		}
		
		//offset++;
		i++;

	}

	localSearchScInvert(s, inst);
	

}
void IteratedLocalSearch::runILSNew(Solution & s, Instance * inst, Config & c)
{
	cplexSolver = new CplexSolver();
	cplexSolver->initialize(inst);

	if (!s.generateInitialSolutionRandom(inst))
	{
		c.output << "No solution" << endl;
		return;
	}

	currentIter = 0;
	c.noImprovement = inst->usCount;
	while (currentIter++ < Config::MAX_ITER && noImprovementCount< c.noImprovement)
	{

		perturbationNew(s, inst);

		localSearchNew(s, inst, c);

		acceptanceCriterionNew(s, inst, c);
	}
	//c.outputExt << currentIter << " " << noImprovementCount << endl;
	//cout << s.bestCost << "  " << currentIter << " " << noImprovementCount << endl;
	c.outputExt << bestSolution.bestCost << endl;
	c.outputExt << "bs: ";
	for (int i = 0; i < bestSolution.bsSet.size(); i++)
	{

		c.outputExt << bestSolution.bsSet[i].first << "->" << bestSolution.bsSet[i].second << " ";
	}
	c.outputExt << endl << "sc: ";
	for (int i = 0; i < bestSolution.scSet.size(); i++)
	{
		c.outputExt << bestSolution.scSet[i] << " ";
	}
	c.outputExt << endl;
	

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
		
		int rand = Config::Rand() % (s.currentBaseStations.size()-1);
		s.insertBsCplex(rand, inst);
		if (cpl->solve(s.B, s.M))
		{
			if ((cost = s.totalCostCplex(inst, cpl)) < s.minLsCost)
			{
				s.minLsCost = cost;
				
				return;
			}
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
		s.insertBsCplex(tempBs - 1, inst);
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
		//cout << "remove - " << s.bsSet[s.bsFixed].first;
		tempBs = s.removeBsCplex(s.bsFixed, inst);
		//cout << " add + " << s.currentBaseStations[tempBs]<<endl;
		if (cpl->solve(s.B, s.M))
		{
			if ((cost = s.totalCostCplex(inst, cpl)) < s.minLsCost)
				s.minLsCost = cost;
			out = s.currentBaseStations[tempBs];
			return true;
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
	int rand = Config::Rand() % (s.bsSet.size()-s.bsFixed) + s.bsFixed;
	s.removeBsCplex(rand, inst);
	rand = Config::Rand() % (s.currentBaseStations.size() - 1);
	s.insertBsCplex(rand, inst);
}

void IteratedLocalSearch::perturbationCplex(Solution & s, Instance * inst, CplexSolver *cpl)
{
	//perturbationBsConnInvert(s);
	//if (currentIter % 5 == 0)
	//{
		perturbationBsInvertCplex(s, inst, cpl);
		//perturbationBsInvertCplex(s, inst, cpl);
	//}
	
	perturbationScInvertCplex(s, inst);
		//localSearchBsAdd(s, inst);
	

	if (noImprovementCount > 5 && noImprovementCount % 5 == 0)
		localSearchBsAddCplex(s, inst);
}
void IteratedLocalSearch::localSearchCplex(Solution & s, Instance * inst, Config & c, CplexSolver *cpl)
{
	localSearchScRemoveCplex(s, inst, cpl);

	//if(noImprovementCount>5)
		if(localSearchBsRemoveCplex(s, inst, cpl))
			return;

	localSearchBsInvertCplex(s, inst, cpl);
	for (int i = 0; i < s.bsSet.size(); i++)
	{
		cout << s.bsSet[i].first << " ";
	}
	cout << endl;
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
		cout << "*** " << s.scSet.size() <<" "<< s.bsSet.size()<<endl;
		perturbationCplex(s, inst, cplexSolver);

		localSearchCplex(s, inst, c, cplexSolver);

		acceptanceCriterionCplex(s, inst, c, cplexSolver);
	}


	//c.outputExt << currentIter << " " << noImprovementCount << endl;
	//cout << currentIter << " " << noImprovementCount << endl << endl;


}