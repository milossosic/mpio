#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Solution.h"
#include "Instance.h"
#include "Config.h"
using namespace std;



Solution::~Solution()
{
}

Solution::Solution()
{
}

Solution::Solution(const Solution & sol)
{
	bestCost = sol.bestCost;
	currentCost = sol.currentCost;
	scSet = deque<int>(sol.scSet);
	bsSet = deque<pair<int, int>>(sol.bsSet);
	bsFixed = sol.bsFixed;
	greedyConn = sol.greedyConn;
	originalBaseStations = deque<baseStation>(sol.originalBaseStations);
	currentBaseStations = deque<int>(sol.currentBaseStations);
	originalSwitchingCenters = deque<switchingCenter>(sol.originalSwitchingCenters);
	currentSwitchingCenters = deque<int>(sol.currentSwitchingCenters);
};

void Solution::initialize(Instance * inst)
{
	greedyConn = false;
	bsFixed = -1;
	//srand(time(NULL));
	
	originalSwitchingCenters.resize(inst->scNewCount + inst->scOldCount);
	
	//for (int i = 0; i < inst->scNewCount + inst->scOldCount;)
	for (int i = 0; i < inst->bsNewCount; i++)
	{
		currentBaseStations.push_back(inst->bsOldCount + i);
	}

	for (int i = 0; i < inst->bsOldCount; i++)
	{
		for (int j = 0; j < inst->scOldCount; j++)
		{
			if (inst->bsScConnMatrix[i][j] == 1)
			{
				originalBaseStations[i].scId = j;
			}
		}
	}

	baseStations = *(new deque<baseStation>(originalBaseStations));
	for (auto i = 0; i < baseStations.size(); i++)
	{
		baseStations[i].id = i;
	}
}

void Solution::bsIdInvert()
{
	int x = this->bsSet.size() - bsFixed;
	if (x <= 1)
		return;
	int randBs1 = Config::Rand() % x + bsFixed;

	int randBs2 = Config::Rand() % x + bsFixed;
	
	while (randBs1 == randBs2)
	{
		randBs2 = Config::Rand() % x + bsFixed;
	}
	int temp = bsSet[randBs1].first;
	bsSet[randBs1].first = bsSet[randBs2].first;
	bsSet[randBs2].first = temp;
}

void Solution::insertRealBs(int id, int scId)
{
	this->bsSet.push_back(make_pair(id, scId));
	this->originalBaseStations[id].scId = scId;
}

int Solution::insertBs(int id, int scId)
{
	int id1 = currentBaseStations[id];
	this->bsSet.push_back(make_pair(id1, scId));
	originalBaseStations[id1].scId = scId;
	//brisanje iz skupa slobodnih bs-a
	currentBaseStations.erase(currentBaseStations.begin() + id);
	return bsSet.size() - 1;
};

int Solution::removeBs(int id)
{
	int id1 = this->bsSet[id].first;
	originalBaseStations[id1].scId = -1;
	currentBaseStations.push_back(id1);
	this->bsSet.erase(this->bsSet.begin() + id);
	return currentBaseStations.size() - 1;
}

void Solution::insertSc(int id)
{
	this->scSet.push_back(currentSwitchingCenters[id]);
	currentSwitchingCenters.erase(currentSwitchingCenters.begin() + id);
};

void Solution::removeSc(int id)
{
	int id1 = this->scSet[id];
	//originalSwitchingCenters[id1].capacity = inst->scCapacity;
	this->scSet.erase(this->scSet.begin() + id);
	currentSwitchingCenters.push_back(id1);
};

int Solution::insertRandomBs(Instance * inst, bool greedy)
{
	int ret;
	//jedan indeks od preostalih bs-ova
	if (currentBaseStations.size() < 1)
		return -1;
	int randBs = Config::Rand() % currentBaseStations.size();
	//FIX ME slucajno povezivanje bs na sc 
	//... moze i greedy, mozda je bolje
	if (!greedy)
	{
		int randConnSc = Config::Rand() % (inst->scOldCount + this->scSet.size());
		if (randConnSc >= inst->scOldCount)
		{
			randConnSc -= inst->scOldCount;
			randConnSc = this->scSet[randConnSc];
		}
		ret = this->insertBs(randBs, randConnSc);
	}
	else
	{
		ret = this->insertBs(randBs,-1);
		setGreedyConn(inst, bsSet.size()-1);
	}

	return ret;
}

int Solution::removeRandomBs()
{
	if (this->bsSet.size() - this->bsFixed < 1)
		return -1;
	int id = Config::Rand() % (this->bsSet.size() - this->bsFixed) + this->bsFixed;
	return removeBs(id);
}

int Solution::insertRandomSc()
{
	if (currentSwitchingCenters.size() == 0)
		return -1;
	int id = Config::Rand() % currentSwitchingCenters.size();
	int id1 = currentSwitchingCenters[id];
	insertSc(id);
	return id1;
}

int Solution::removeRandomSc()
{
	if (scSet.size() == 0)
		return -1;
	int id = Config::Rand() % scSet.size();
	int id1 = scSet[id];
	removeSc(id);
	return id1;
}

void Solution::genInitScSet(Instance * inst)
{
	int n = inst->usCount / inst->scCapacity + 2 - inst->scOldCount;
	if (n > inst->scNewCount)
		n = inst->scNewCount;
	for (int i = 0; i < n; i++)
	{
		this->scSet.push_back(i + inst->scOldCount);
	}
}

int Solution::totalCost(Instance * inst)
{
	int cost = this->bsSet.size()*inst->bsCost + this->scSet.size()*inst->scCost;
	int x, y;
	for (int i = 0; i < this->bsSet.size(); i++)
	{
		x = this->bsSet[i].first - inst->bsOldCount;
		y = this->bsSet[i].second;
		cost += inst->bsScConnCost[x][y];
	}
	return cost;
};

void Solution::generateBsMustSet(Instance * inst)
{
	int covered = 0;
	int bsId;
	bool flag;
	//ako se prvi put izracunava koje bs-e moraju biti u svakom resenju
	if (this->bsFixed == -1)
	{
		//korisnici pokriveni samo jednom bs - ta bs mora biti u resenju
		for (int i = 0; i < inst->usCount; i++)
		{
			if (inst->users[i].bsSet.size() == 1)
			{
				bsId = inst->users[i].bsSet[0].first;
				if (bsId < inst->bsOldCount)
					continue;
				flag = false;
				//provera da li je bs vec u resenju
				for (int j = 0; j < this->bsSet.size(); j++)
				{
					if (this->bsSet[j].first == bsId)
						flag = true;
				}
				if (flag)
					continue;
				
				insertRealBs(bsId,-1);

				//povecava se broj bs-a
				covered++;
			}
		}
		this->bsFixed = covered;
		
	}
}

void Solution::setGreedyConn(Instance *inst, int bsId)
{
	vector<pair<int,int>> bsScConnList;
	//stari sc
	for (int i = 0; i < inst->scOldCount; i++)
	{
		bsScConnList.push_back(make_pair(i, inst->bsScConnCost[bsSet[bsId].first][i]));
	}
	//novi sc
	for (int i = 0; i < scSet.size(); i++)
	{
		bsScConnList.push_back(make_pair(scSet[i], inst->bsScConnCost[bsSet[bsId].first][scSet[i]]));
	}
	sort(bsScConnList.begin(), bsScConnList.end(), Config::comparePairs);

	int rand = Config::Rand() % 10;
	if (rand < 7)
	{
		bsSet[bsId].second = bsScConnList[0].first;
		originalBaseStations[bsSet[bsId].first].scId = bsScConnList[0].first;
	}
	else
		if (bsScConnList.size()>1)
		{
			bsSet[bsId].second = bsScConnList[1].first;
			originalBaseStations[bsSet[bsId].first].scId = bsScConnList[1].first;
		}
}

void Solution::setRandomScConn(int bsId, Instance * inst, bool greedy)
{
	if (!greedy)
	{
		int randConnSc = Config::Rand() % (inst->scOldCount + this->scSet.size());
		if (randConnSc >= inst->scOldCount)
		{
			randConnSc -= inst->scOldCount;
			randConnSc = this->scSet[randConnSc];
		}
		
		this->bsSet[bsId].second = randConnSc;
		this->originalBaseStations[bsSet[bsId].first].scId = randConnSc;
	}
	else
	{
		setGreedyConn(inst, bsId);
	}
}

void Solution::resetCapacities(Instance * inst)
{
	for (int i = 0; i < originalSwitchingCenters.size(); i++)
	{
		originalSwitchingCenters[i].capacity = inst->scCapacity;
	}
	for (int i = 0; i < originalBaseStations.size(); i++)
	{
		originalBaseStations[i].capacity = inst->bsCapacity;
	}
}

void Solution::resetBsScIds(Instance * inst)
{
	for (int i = inst->bsOldCount; i < inst->bsOldCount+inst->bsNewCount; i++)
	{
		this->originalBaseStations[i].scId = -1;
	}
}

void Solution::resetBs(Instance * inst)
{
	resetBsScIds(inst);
	currentBaseStations.clear();
	for (int i = inst->bsOldCount; i < inst->bsOldCount + inst->bsNewCount; i++)
	{
		currentBaseStations.push_back(i);
	}
	//brisu se obavezne bs-e
	if (bsFixed != -1)
	{
		for (int i = 0; i < bsFixed; i++)
		{
			int j = bsSet[i].first;
			for (int k = 0; k < currentBaseStations.size(); k++)
			if (currentBaseStations[k] == j)
			{
				currentBaseStations.erase(currentBaseStations.begin() + k);
				break;
			}
		}

		for (int i = bsSet.size() - 1; i >= bsFixed; i--)
		{
			originalBaseStations[bsSet[i].first].scId = -1;
			bsSet.erase(bsSet.begin() + i);

		}
	}
}

void Solution::genInitBsSet(Instance * inst, int bsN)
{
	
	if (bsFixed == -1)
		generateBsMustSet(inst);
	
	resetBs(inst);
	
	//za fiksirane bs bira se slucajno samo konekcija ka sc
	for (int i = 0; i < this->bsFixed; i++)
	{
		////FIX ME slucajno povezivanje bs na sc 
		////... moze i greedy, mozda je bolje
		setRandomScConn(i,inst,greedyConn);
		//greedyConn(bsId,i);
	}
	//za ostale bs, bira se na slucajan nacin polovina koja ulazi u pocetno resenje
	for (int i = 0; i < bsN; i++)
	{
		insertRandomBs(inst,greedyConn);
	}
}

bool Solution::coverUsers(Instance * inst)
{
	int randId;
	bool coverPossible = true;
	int ind, i, firstAvailable, bsInd;

	//kapaciteti
	resetCapacities(inst);

	//10 random startova - pokusaja pokrivanja korisnika datim skupom baznih stanica
	for (int n = 0; coverPossible && n < 5; n++)
	{
		randId = Config::Rand() % inst->usCount;

		for (i = 0; i < inst->usCount; i++)
		{
			ind = (i + randId) % inst->usCount;
			//trazi se prva slobodna bs, pocevsi od najblize,
			//koja zajedno sa njenim odgovarajucim sc-om moze da pokrije trenutnog korisnika
			for (firstAvailable = 0; firstAvailable < inst->users[ind].bsSet.size(); firstAvailable++)
			{
				bsInd = inst->users[ind].bsSet[firstAvailable].first;

				if (originalBaseStations[bsInd].scId == -1)
				{
					//bs nije u resenju
					continue;
				}
				if (originalBaseStations[bsInd].capacity > 0 && originalSwitchingCenters[originalBaseStations[bsInd].scId].capacity > 0)
				{
					originalBaseStations[bsInd].capacity--;
					originalSwitchingCenters[originalBaseStations[bsInd].scId].capacity--;
					break;
				}

			}

			if (firstAvailable == inst->users[ind].bsSet.size())
			{
				//nije nadjen odgovarajuci bs
				coverPossible = false;
				break;
			}
		}
		//ako su pokriveni svi korisnici
		if (i == inst->usCount)
		{
			return true;
		}
		//inace prelazak na sledecu iteraciju
	}
	return false;
}

void Solution::resetSolution(Instance * inst)
{
	resetBs(inst);
	this->scSet.clear();
}

void Solution::generateInitialSolutionRandom(Instance * inst)
{
	int i = 0, bsN;
	
	generateBsMustSet(inst);
	bsN = ((inst->bsNewCount - this->bsFixed) / 2 > 5) ? 5 : (inst->bsNewCount - this->bsFixed) / 2;
	do
	{
		resetSolution(inst);
		genInitScSet(inst);
		genInitBsSet(inst,bsN);
		i++;
		if (i % 4 == 0)
			bsN++;
	} while (!coverUsers(inst));
	
	bestCost = totalCost(inst);
	//cout << "initial solution generated after " << i << " attempts" << endl;
	//output << "initial solution generated after "<<i<<" attempts" << endl;

}

void Solution::generateInitialSolutionGreedy(Instance * inst)
{
	int bsId,n;
	bool empty = false;
	generateBsMustSet(inst);
	genInitScSet(inst);
	for (int i = 0; i < inst->bsOldCount; i++)
	{
		baseStations[0].capacity = inst->bsCapacity;
		
		auto j = baseStations[0].users.begin();
		while (baseStations[0].capacity > 0 && j != baseStations[0].users.end())
		{
			for (int k = 1; k < baseStations.size(); k++)
				baseStations[k].users.erase(*j);
			j++;
			baseStations[0].capacity--;
		}
		baseStations.erase(baseStations.begin());
	}
	while (baseStations.size()!=0)
	{
		sortBs(baseStations);
		n = 0;
		while (n < baseStations.size() && baseStations[n].users.size() != 0)
			n++;
		baseStations.erase(baseStations.begin() + n,baseStations.end());
		if (baseStations.size() == 0)
		{
			bestCost = totalCost(inst);
			return;
		}

		bsId = baseStations[0].id;
		

		for (auto j = baseStations[0].users.begin(); j != baseStations[0].users.end(); j++)
		{
			for (int k = 1; k < baseStations.size(); k++)
				baseStations[k].users.erase(*j);
		}
		baseStations.erase(baseStations.begin());
		bsSet.push_back(make_pair(bsId, -1));
		setRandomScConn(bsSet.size()-1, inst, greedyConn);
	}

	bestCost = totalCost(inst);
}