#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Solution.h"
#include "Instance.h"
#include "Config.h"

#include "CplexSolver.h"
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

	for (int i = 0; i < inst->scNewCount; i++)
	{
		currentSwitchingCenters.push_back(inst->scOldCount + i);
	}

	baseStations = *(new deque<baseStation>(originalBaseStations));
	
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

int Solution::insertBsNew(int id, int scId, Instance *inst)
{
	int id1 = currentBaseStations[id];
	this->bsSet.push_back(make_pair(id1, scId));
	originalBaseStations[id1].scId = scId;
	//brisanje iz skupa slobodnih bs-a
	currentBaseStations.erase(currentBaseStations.begin() + id);
	return bsSet.size() - 1;
};

void Solution::insertRealBsCplex(int id, Instance *inst)
{
	bsSet.push_back(make_pair(id, -1));
	B[id - inst->bsOldCount] = 1;
	for (int i = 0; i < currentBaseStations.size(); i++)
	{
		if (currentBaseStations[i] == id)
		{
			currentBaseStations.erase(currentBaseStations.begin() + i);
			return;
		}
	}
}

void Solution::removeRealBsCplex(int id, Instance *inst)
{
	currentBaseStations.push_back(id);
	B[id - inst->bsOldCount] = 0;
	for (int i = bsFixed; i < bsSet.size(); i++)
	{
		if (bsSet[i].first == id)
		{
			bsSet.erase(bsSet.begin() + i);
			return;
		}
	}
}

int Solution::insertBsCplex(int id, Instance * inst)
{
	int id1 = currentBaseStations[id];
	this->bsSet.push_back(make_pair(id1, -1));
	B[id1 - inst->bsOldCount] = 1;
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

int Solution::removeBsCplex(int id,Instance *inst)
{
	int id1 = this->bsSet[id].first;
	//originalBaseStations[id1].scId = -1;
	B[id1 - inst->bsOldCount] = 0;
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
int Solution::removeRandomBsNew(Instance * inst)
{
	if (this->bsSet.size() - this->bsFixed < 1)
		return -1;
	int id = Config::Rand() % (this->bsSet.size() - this->bsFixed) + this->bsFixed;
	currentCost -= inst->bsCost + inst->bsScConnCost[bsSet[id].first - inst->bsOldCount][bsSet[id].second];
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
		insertSc(0);
	}
}
void Solution::genInitScSetCplex(Instance * inst)
{
	int n = inst->usCount / inst->scCapacity + 2 - inst->scOldCount;
	if (n > inst->scNewCount)
		n = inst->scNewCount;
	for (int i = 0; i < n; i++)
	{
		M[i] = 1;
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
int Solution::totalCostCplex(Instance * inst, CplexSolver * cpl)
{
	int cost = this->bsSet.size()*inst->bsCost + this->scSet.size()*inst->scCost;
	cost += cpl->objValue;
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
void Solution::generateBsMustSetCplex(Instance * inst)
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

				B[bsId - inst->bsOldCount] = 1;
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
	sort(bsScConnList.begin(), bsScConnList.end(), Config::comparePairsAsc);

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

		bsSet.erase(bsSet.begin() + bsFixed, bsSet.end());
	}
}
void Solution::resetSc(Instance* inst)
{
	scSet.clear();
	currentSwitchingCenters.clear();
	for (int i = 0; i < inst->scNewCount; i++)
	{
		currentSwitchingCenters.push_back(inst->scOldCount+i);
	}
}
void Solution::resetBsCplex(Instance * inst)
{
	
	currentBaseStations.clear();
	for (int i = inst->bsOldCount; i < inst->bsOldCount + inst->bsNewCount; i++)
	{
		currentBaseStations.push_back(i);
		B[i - inst->bsOldCount] = 0;
	}
	//brisu se obavezne bs-e
	if (bsFixed != -1)
	{
		for (int i = 0; i < bsFixed; i++)
		{
			int j = bsSet[i].first;
			B[j - inst->bsOldCount] = 1;
			for (int k = 0; k < currentBaseStations.size(); k++)
				if (currentBaseStations[k] == j)
				{
					currentBaseStations.erase(currentBaseStations.begin() + k);
					break;
				}
		}

		
		bsSet.erase(bsSet.begin() + bsFixed,bsSet.end());

		
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

	//currentCost = totalCost(inst);
}
void Solution::genInitBsSetCplex(Instance * inst, int bsN)
{
	resetBsCplex(inst);
	//za ostale bs, bira se na slucajan nacin polovina koja ulazi u pocetno resenje
	for (int i = 0; i < bsN; i++)
	{
		insertRandomBs(inst, greedyConn);
		B[bsSet[bsSet.size() - 1].first - inst->bsOldCount] = 1;
	}
}
bool Solution::coverUsers(Instance * inst)
{
	int randId;
	bool coverPossible = true, covered;
	int ind, i, firstAvailable, bsInd;

	//kapaciteti
	resetCapacities(inst);
	resetBsScIds(inst);
	for (int i = 0; i < bsSet.size(); i++)
	{
		originalBaseStations[bsSet[i].first].scId = bsSet[i].second;
	}
	//5 random startova - pokusaja pokrivanja korisnika datim skupom baznih stanica
	for (int n = 0; coverPossible && n < 5; n++)
	{
		randId = Config::Rand() % inst->usCount;
		coverPossible = true;
		for (i = 0; i < inst->usCount; i++)
		{
			ind = (i + randId) % inst->usCount;
			//trazi se prva slobodna bs, pocevsi od najblize,
			//koja zajedno sa njenim odgovarajucim sc-om moze da pokrije trenutnog korisnika
			covered = false;
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
					covered = true;
					break;
				}

			}

			if (!covered)
			{
				//nije nadjen odgovarajuci bs
				coverPossible = false;
				break;
			}
		}
		//ako su pokriveni svi korisnici
		if (i == inst->usCount && coverPossible)
		{
			return true;
		}
		//inace prelazak na sledecu iteraciju
	}
	return false;
}


bool Solution::distributeScId(int id)
{
	//  FIX ME
	return true;
}
/*  inicijalizuje trenutne sc-ove  */
void Solution::selectScInit(Instance * inst)
{
	scs.clear();
	for (int i = 0; i < originalSwitchingCenters.size(); i++)
	{
		originalSwitchingCenters[i].id = -1;
	}
	for (int i = 0; i < bsSet.size(); i++)
	{
		originalSwitchingCenters[bsSet[i].second].id = 1;
	}
	for (int i = 0; i < inst->bsOldCount; i++)
	{
		originalSwitchingCenters[originalBaseStations[i].scId].id = 1;
	}
	for (int i = 0; i < originalSwitchingCenters.size(); i++)
	{
		if (originalSwitchingCenters[i].id == 1 )
		{
			scs.push_back(make_pair(i, originalSwitchingCenters[i].capacity));
		}
	}
}
/*  inicijalizuje trenutne bs-ove  */
void Solution::selectBsInit()
{
	//FIX ME
	for (int i = 0; i < bsSet.size(); i++)
	{
		bss.push_back(make_pair(bsSet[i].first, bsSet[i].second));
	}
}
int Solution::selectSc(int id)
{
	sort(scs.begin(),scs.end(),Config::comparePairsDsc);
	return 0;
}
bool Solution::selectBs(int id, Instance *inst)
{
	int bsId,bsSetId,i;
	vector<pair<int, int>> tempBs;
	
	sort(scs.begin(), scs.end(), Config::comparePairsDsc);
	//	prioritet ima popunjenost sc-ova
	/*int sum = 0, rand;
	for (int i = 0; i < scs.size(); i++)
	{
		sum += originalSwitchingCenters[scs[i].first].capacity;
	}

	if (sum == 0)
		return false;
	rand = Config::Rand() % sum;*/
	int offset=0;
	/*for (int i = 0; i < scs.size(); i++)
	{
		if (rand < scs[i].second)
		{
			
		}
		else
		{
			offset++;
			rand -= scs[i].second;
		}
	}*/
	for (i = 0; i < scs.size(); i++)
	{
		int bsTemp = (offset + i) % scs.size();
		tempBs.clear();
		for (int j = 0; j < inst->users[id].bsSet.size(); j++)
		{
			if (originalBaseStations[inst->users[id].bsSet[j].first].scId == scs[bsTemp].first && \
				originalBaseStations[inst->users[id].bsSet[j].first].capacity > 0 && \
				originalSwitchingCenters[scs[bsTemp].first].capacity > 0)
			{
				//	parovi <bsId,capacity>
				tempBs.push_back(make_pair(inst->users[id].bsSet[j].first, originalBaseStations[inst->users[id].bsSet[j].first].capacity));
			}
		}
		if (tempBs.size() == 0)
			continue;
		// FIX ME
		// dovoljno je minimalni da se izabere, nije potrebno sortiranje
		sort(tempBs.begin(),tempBs.end(),Config::comparePairsDsc);

		bsId = tempBs[0].first;
		scs[bsTemp].second--;
		originalBaseStations[bsId].capacity--;
		originalSwitchingCenters[originalBaseStations[bsId].scId].capacity--;
		return true;
	}
	
	return false;
}
bool Solution::coverUsersNew(Instance * inst)
{
	resetCapacities(inst);
	resetBsScIds(inst);
	for (int i = 0; i < bsSet.size(); i++)
	{
		originalBaseStations[bsSet[i].first].scId = bsSet[i].second;
	}

	selectScInit(inst);
	//selectBsInit();
	for (int i = 0; i < inst->usCount; i++)
	{
		if (!selectBs(i, inst))
		{
			return false;
		}
	}

	return true;
}
bool Solution::coverUsersCplex(Instance * inst)
{
	return false;
}
void Solution::resetSolution(Instance * inst)
{
	resetBs(inst);
	resetSc(inst);
}
void Solution::resetSolutionCplex(Instance * inst)
{

	resetBsCplex(inst);
	this->scSet.clear();
	for (int i = 0; i < M.size(); i++)
		M[i] = 0;
}
bool Solution::generateInitialSolutionRandom(Instance * inst)
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

		if (bsN > inst->bsNewCount && i > 20)
			return false;
	} while (!coverUsersNew(inst));
	currentCost = totalCost(inst);
	bestCost = currentCost;
	return true;
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

void Solution::generateInitialSolutionCplex(Instance * inst, CplexSolver * cpl)
{
	int i = 0, bsN;

	generateBsMustSetCplex(inst);
	bsN = (inst->bsNewCount - this->bsFixed) / 2;
	do
	{
		resetSolutionCplex(inst);
		genInitScSetCplex(inst);
		genInitBsSetCplex(inst, bsN);
		i++;
		if (bsN % 2 == 0)
			bsN++;
	} while (!cpl->solve(B,M));

	bestCost = totalCostCplex(inst, cpl);
	//cout << "initial solution generated after " << i << " attempts" << endl;
	//output << "initial solution generated after "<<i<<" attempts" << endl;

}
