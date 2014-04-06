#include <iostream>
#include <cstdlib>
#include "Solution.h"

using namespace std;



Solution::~Solution()
{
}

Solution::Solution()
{
}

Solution::Solution(Solution & sol)
{
	scSet = deque<int>(sol.scSet);
	bsSet = deque<pair<int, int>>(sol.bsSet);
	bsFixed = sol.bsFixed;
};

void Solution::insertBs(int id, int scId)
{
	int id1 = currentBaseStations[id];
	this->bsSet.push_back(make_pair(id1, scId));
	originalBaseStations[id1].scId = scId;
	//brisanje iz skupa slobodnih bs-a
	currentBaseStations.erase(currentBaseStations.begin() + id);

};

void Solution::removeBs(int id)
{
	int id1 = this->bsSet[id].first;
	originalBaseStations[id1].scId = -1;
	currentBaseStations.push_back(id1);
	this->bsSet.erase(this->bsSet.begin() + id);
}

void Solution::insertSc(int id)
{
	this->scSet.push_back(currentSwitchingCenters[id]);
	currentSwitchingCenters.erase(currentSwitchingCenters.begin() + id);
};

void Solution::removeSc(int id)
{
	int id1 = this->scSet[id];
	originalSwitchingCenters[id1].capacity = inst.scCapacity;
	this->scSet.erase(this->scSet.begin() + id);
	currentSwitchingCenters.push_back(id1);
};

void Solution::insertRandomBs()
{
	//jedan indeks od preostalih bs-ova
	int randBs = rand() % currentBaseStations.size();
	//indeks izabrane bs
	randBs = currentBaseStations[randBs];
	//FIX ME slucajno povezivanje bs na sc 
	//... moze i greedy, mozda je bolje

	int randConnSc = rand() % (inst.scOldCount + this->scSet.size());
	if (randConnSc >= inst.scOldCount)
	{
		randConnSc -= inst.scOldCount;
		randConnSc = this->scSet[randConnSc];
	}

	this->insertBs(randBs, randConnSc);
}

void Solution::removeRandomBs()
{
	int id = rand() % (this->bsSet.size() - this->bsFixed) + this->bsFixed;
	removeBs(id);
}

void Solution::insertRandomSc()
{
	if (currentSwitchingCenters.size() == 0)
		return;
	int id = rand() % currentSwitchingCenters.size();
	insertSc(id);
}

void Solution::removeRandomSc()
{
	if (scSet.size() == 0)
		return;
	int id = rand() % scSet.size();
	removeSc(id);
}

void Solution::genInitScSet()
{
	for (int i = 0; i < inst.scNewCount; i++)
	{
		this->scSet.push_back(i + inst.scOldCount);
	}
}

int Solution::totalCost()
{
	int cost = this->bsSet.size()*inst.bsCost + this->scSet.size()*inst.scCost;
	int x, y;
	for (int i = 0; i < this->bsSet.size(); i++)
	{
		x = this->bsSet[i].first;
		y = this->bsSet[i].second;
		cost += inst.bsScConnCost[x][y];
	}
	return cost;
};

void Solution::generateBsMustSet()
{
	int covered = 0;
	int bsId;
	bool flag;
	//ako se prvi put izracunava koje bs-e moraju biti u svakom resenju
	if (this->bsFixed == -1)
	{
		//korisnici pokriveni samo jednom bs - ta bs mora biti u resenju
		for (int i = 0; i < inst.usCount; i++)
		{
			if (inst.users[i].bsSet.size() == 1)
			{
				bsId = inst.users[i].bsSet[0].first;
				flag = false;
				//provera da li je bs vec u resenju
				for (int j = 0; j < this->bsSet.size(); j++)
				{
					if (this->bsSet[j].first == bsId)
						flag = true;
				}
				if (flag)
					continue;
				
				insertBs(bsId,-1);

				//povecava se broj bs-a
				covered++;
			}
		}
		this->bsFixed = covered;
		
	}
}

//void greedyConn(int bsId, int i)
//{
//	bsScConnList.clear();
//	for (int i = 0; i < scOldCount; i++)
//	{
//		bsScConnList.push_back(make_pair(i, bsScConnCost[bsId][i]));
//	}
//	for (int i = 0; i < solution.scSet.size(); i++)
//	{
//		bsScConnList.push_back(make_pair(i, bsScConnCost[bsId][solution.scSet[i]]));
//	}
//	sort(bsScConnList.begin(), bsScConnList.end(), comparePairs);
//
//	uniform_int_distribution<int> distConn(0, 1);
//	int dice = (distConn(generator) + distConn(generator)) / 2;
//	if (i != -1)
//	{
//		solution.bsSet[i].second = bsScConnList[dice].first;
//		originalBaseStations[bsId].scId = bsScConnList[dice].first;
//	}
//	else
//	{
//		solution.bsSet.push_back(make_pair(bsId, bsScConnList[dice].first));
//		originalBaseStations[bsId].scId = bsScConnList[dice].first;
//	}
//}

void Solution::setRandomScConn(int i)
{
	int randConnSc = rand() % (inst.scOldCount + this->scSet.size());
	if (randConnSc >= inst.scOldCount)
	{
		randConnSc -= inst.scOldCount;
		randConnSc = this->scSet[randConnSc];
	}
	this->bsSet[i].second = randConnSc;
	this->originalBaseStations[bsSet[i].first].scId = randConnSc;
}

void Solution::resetCapacities()
{
	for (int i = 0; i < originalSwitchingCenters.size(); i++)
	{
		originalSwitchingCenters[i].capacity = inst.scCapacity;
	}
	for (int i = 0; i < originalBaseStations.size(); i++)
	{
		originalBaseStations[i].capacity = inst.bsCapacity;
	}
}

void Solution::resetBsScIds()
{
	for (int i = inst.bsOldCount; i < inst.bsOldCount+inst.bsNewCount; i++)
	{
		this->originalBaseStations[i].scId = -1;
	}
}

void Solution::resetBs()
{
	resetBsScIds();
	currentBaseStations.clear();
	for (int i = inst.bsOldCount; i < inst.bsOldCount + inst.bsNewCount; i++)
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

void Solution::genInitBsSet()
{
	int bsId, randConnSc;
	if (bsFixed == -1)
		generateBsMustSet();
	
	//resetCapacities;
	resetBs();
	//resetBsScIds();
	//za fiksirane bs bira se slucajno samo konekcija ka sc
	for (int i = 0; i < this->bsFixed; i++)
	{
		////FIX ME slucajno povezivanje bs na sc 
		////... moze i greedy, mozda je bolje
		setRandomScConn(i);
		//greedyConn(bsId,i);
	}
	//za ostale bs, bira se na slucajan nacin polovina koja ulazi u pocetno resenje
	for (int i = 0; i < (inst.bsNewCount - this->bsFixed) / 2; i++)
	{
		insertRandomBs();
	}
}

bool Solution::coverUsers()
{
	int randId;
	bool coverPossible = true;
	int ind, i, firstAvailable, bsInd;

	//kapaciteti
	resetCapacities();

	//10 random startova - pokusaja pokrivanja korisnika datim skupom baznih stanica
	for (int n = 0; coverPossible && n < 10; n++)
	{
		randId = rand() % inst.usCount;

		for (i = 0; i < inst.usCount; i++)
		{
			ind = (i + randId) % inst.usCount;
			//trazi se prva slobodna bs, pocevsi od najblize,
			//koja zajedno sa njenim odgovarajucim sc-om moze da pokrije trenutnog korisnika
			for (firstAvailable = 0; firstAvailable < inst.users[ind].bsSet.size(); firstAvailable++)
			{
				bsInd = inst.users[ind].bsSet[firstAvailable].first;

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

			if (firstAvailable == inst.users[ind].bsSet.size())
			{
				//nije nadjen odgovarajuci bs
				coverPossible = false;
				break;
			}
		}
		//ako su pokriveni svi korisnici
		if (i == inst.usCount)
		{
			return true;
		}
		//inace prelazak na sledecu iteraciju
	}
	return false;
}

void Solution::resetSolution()
{
	resetBs();
	this->scSet.clear();
}

void Solution::generateInitialSolution()
{
	int i = 0;
	generateBsMustSet();
	do
	{
		resetSolution();
		genInitScSet();
		genInitBsSet();
		i++;
	} while (!coverUsers());
	bestCost = totalCost();
	//cout << "initial solution generated after " << i << " attempts" << endl;
	//output << "initial solution generated after "<<i<<" attempts" << endl;

}
