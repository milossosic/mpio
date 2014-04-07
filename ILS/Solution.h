#pragma once
#include <queue>
#include <utility>
#include "Instance.h"
#include "switchingCenter.h"
#include "baseStation.h"
using namespace std;
class Solution
{
public:
	int bestCost;
	int currentCost;

	
	deque<int> scSet;
	deque<pair<int, int>> bsSet;
	int bsFixed;
	
	deque<baseStation> originalBaseStations;
	deque<int> currentBaseStations;
	deque<switchingCenter> originalSwitchingCenters;
	deque<int> currentSwitchingCenters;
	

	//deque<pair<int, int>> bsScConnList;

	Solution();
	Solution(const Solution & sol);
	~Solution();

	void insertRealBs(int id, int scId);


	void initialize(Instance * inst);

	void bsIdInvert();

	//indeks u currentBS
	void insertBs(int id, int scId);

	// indeks u bsSet - u
	void removeBs(int id);

	// id je redni broj sc-a u currentSwitchingCenters
	void insertSc(int id);

	// id - redni broj sc-a u scSet-u
	void removeSc(int id);

	void insertRandomBs(Instance * inst);

	void removeRandomBs();

	int insertRandomSc();

	int removeRandomSc();

	
	void resetBs(Instance * inst);

	void resetSolution(Instance * inst);

	void setRandomScConn(int id, Instance * inst);

	void genInitScSet(Instance * inst);

	int totalCost(Instance * inst);

	void generateBsMustSet(Instance * inst);

	void genInitBsSet(Instance * inst);

	bool coverUsers(Instance * inst);

	void resetBsScIds(Instance * inst);

	void resetCapacities(Instance * inst);


	void generateInitialSolution(Instance * inst);
};