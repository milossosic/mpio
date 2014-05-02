#include <iostream>
#include <utility>
#include <deque>
#include "IteratedLocalSearch.h"
#include "Config.h"
#include "Solution.h"
#include "Instance.h"
IteratedLocalSearch::IteratedLocalSearch(){}
IteratedLocalSearch::~IteratedLocalSearch(){}

void IteratedLocalSearch::localSearchBsAdd(Solution & s, Instance * inst)
{
	if (s.currentBaseStations.size() == 0)
		return;
	s.insertRandomBs(inst);
}


void IteratedLocalSearch::localSearchBsInvertNew(Solution & s, Instance * inst)
{

	if (s.bsFixed == s.bsSet.size())
		return;
	

	int randInt2,randInt1 = Config::Rand() % (s.bsSet.size() - s.bsFixed) + s.bsFixed;
	int scId = (s.bsSet[randInt1].second + 1)%(inst->scOldCount + s.scSet.size());
	if (scId > inst->scOldCount) scId = s.scSet[scId - inst->scOldCount];
	randInt2 = Config::Rand() % s.currentBaseStations.size();
	s.removeBs(randInt1);

	s.insertBs(randInt2,-1);
	s.setRandomScConn(s.bsSet.size() - 1, inst);
	
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
		tempBs = s.removeBs(s.bsFixed);

		if (s.coverUsersNew(inst))
		{
			return true;
		}

		s.insertBs(tempBs, -1);
		s.setRandomScConn(s.bsSet.size() - 1, inst);
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
	bool notInBs = true,newSc = false;
	int scOld, scNew, oldCost, newCost, sum=0;
	for (int i = 0; i < s.bsSet.size(); i++)
	{
		if (s.bsSet[i].second >= inst->scOldCount)
		{
			newSc = true;
		}
	}
	if (!newSc)
		return false;
	for (int k = 0; k < s.scSet.size(); k++)
	{
		scOld = s.scSet[k];
		for (int j = 0; j < s.currentSwitchingCenters.size(); j++)
		{
			//int randInt = Config::Rand() % s.currentSwitchingCenters.size();
			//randInt = (randInt + j) % s.currentSwitchingCenters.size();
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
				break;
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
				s.currentSwitchingCenters.push_back(scOld);
				return true;
			}
		}
	}
	return false;
}
bool IteratedLocalSearch::localSearchScInvertConn(Solution & s, Instance * inst)
{
	int sum = 0;
	int bsId, scId1,scId2, oldCost, newCost;
	for (int i = 0; i < s.bsSet.size(); i++)
	{
		bsId = s.bsSet[i].first - inst->bsOldCount;
		scId1 = s.bsSet[i].second;
		int randInt = Config::Rand();
		for (int j = 0; j < inst->scOldCount + s.scSet.size(); j++)
		{
			scId2 = (randInt + j) % (inst->scOldCount + s.scSet.size());
			scId2 = scId2 < inst->scOldCount ? scId2 : s.scSet[scId2-inst->scOldCount];
			oldCost = inst->bsScConnCost[bsId][scId1];
			newCost = inst->bsScConnCost[bsId][scId2];
			if (newCost < oldCost && s.coverUsersNew(inst))
			{
				s.bsSet[i].second = scId2;
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
	int id2 = s.insertRandomSc();

	for (int i = 0; i < s.bsSet.size(); i++)
	{
		if (s.bsSet[i].second == id1)
		{
			s.bsSet[i].second = id2;
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
	if (noImprovementCount > 5 && currentIter % 5 == 4)
	{
		localSearchBsInvertNew(s, inst);
		localSearchBsInvertNew(s, inst);
		localSearchBsRemove(s, inst);
	}
	if (currentIter%5 == 9)
		perturbationScInvertNew(s, inst);
	
}
void IteratedLocalSearch::localSearchNew(Solution & s, Instance * inst)
{
	bool better = false;
	int oldCost, newCost;
	int i, randInt;
	if (inst->usCount <= (inst->scOldCount + s.scSet.size())*inst->scCapacity - inst->scCapacity)
	{
		if (localSearchScRemove(s, inst))
		{
			return;
		}
	}

	if (localSearchBsRemove(s, inst))
	{
		return;
	}
	
	if (localSearchScInvert(s,inst))
	{
		return;
	}

	if (localSearchScInvertConn(s, inst))
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
				return;
			}
		}
	}

	

	while (i < s.bsSet.size())
	{
		int offset = 0;
		randInt = Config::Rand() % s.currentBaseStations.size();
		for (int i = 0; i < Config::lsDepth; i++)
		{			
			randInt = (i + randInt) % s.currentBaseStations.size();
			
			for (int j = 0; j < inst->scOldCount + s.scSet.size(); j++)
			{
				int scId = j < inst->scOldCount ? j : s.scSet[j - inst->scOldCount];
				oldCost = inst->bsScConnCost[s.bsSet[s.bsFixed + offset].first - inst->bsOldCount][s.bsSet[s.bsFixed + offset].second];
				newCost = inst->bsScConnCost[s.currentBaseStations[randInt] - inst->bsOldCount][scId];
				if (newCost < oldCost)
				{
					//offset--;
					int tempSc = s.bsSet[s.bsFixed + offset].second;
					s.removeBs(s.bsFixed + offset);
					s.insertBs(randInt, scId);
					if (s.coverUsersNew(inst))
					{
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
		
		localSearchNew(s, inst);
		
		acceptanceCriterionNew(s, inst, c);
		
	}
	
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