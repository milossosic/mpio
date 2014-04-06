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

	Instance inst;
	deque<int> scSet;
	deque<pair<int, int>> bsSet;
	int bsFixed;
	
	deque<baseStation> originalBaseStations;
	deque<int> currentBaseStations;
	deque<switchingCenter> originalSwitchingCenters;
	deque<int> currentSwitchingCenters;
	

	//deque<pair<int, int>> bsScConnList;

	Solution();
	Solution(Solution & sol);
	~Solution();

	void bsIdInvert();

	//indeks u currentBS
	void insertBs(int id, int scId);

	// indeks u bsSet - u
	void removeBs(int id);

	// id je redni broj sc-a u currentSwitchingCenters
	void insertSc(int id);

	// id - redni broj sc-a u scSet-u
	void removeSc(int id);

	void insertRandomBs();

	void removeRandomBs();

	int insertRandomSc();

	int removeRandomSc();

	
	void resetBs();

	void resetSolution();

	void setRandomScConn(int id);

	void genInitScSet();

	int totalCost();

	void generateBsMustSet();

	void genInitBsSet();

	bool coverUsers();

	void resetBsScIds();

	void resetCapacities();


	void generateInitialSolution();
};