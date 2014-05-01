#pragma once
#include <queue>
#include <utility>
#include <cstdlib>
#include "Instance.h"
#include "switchingCenter.h"
#include "baseStation.h"
using namespace std;
class Solution
{
public:
	int bestCost;
	int currentCost;
	
	double eps = 0.0001;
	vector<pair<int, int>> scs;
	vector<pair<int, int>> bss;
	deque<int> scSet;
	deque<pair<int, int>> bsSet;
	int bsFixed;
	
	deque<baseStation> originalBaseStations;
	deque<int> currentBaseStations;
	deque<switchingCenter> originalSwitchingCenters;
	deque<int> currentSwitchingCenters;
	
	static bool compareBs(const baseStation & a, const baseStation & b)
	{
		return a.users.size()>b.users.size();
	}

	static void sortBs(deque<baseStation> & baseStations)
	{
		sort(baseStations.begin(), baseStations.end(), compareBs);
	}


	Solution();
	Solution(const Solution & sol);
	~Solution();

	void insertRealBs(int id, int scId);


	void initialize(Instance * inst);

	int insertBs(int id, int scId);
	int removeBs(int id);
	void insertSc(int id);
	void removeSc(int id);
	int insertRandomBs(Instance * inst);
	int removeRandomBs();
	int insertRandomSc();
	int removeRandomSc();

	void resetBs(Instance * inst);

	void resetSolution(Instance * inst);

	void resetSc(Instance * inst);

	void setRandomScConn(int id, Instance * inst);
	void genInitScSet(Instance * inst);
	int totalCost(Instance * inst);
	void generateBsMustSet(Instance * inst);
	void genInitBsSet(Instance * inst, int bsN);


	bool distributeScId(int id);
	void selectScInit(Instance * inst);
	void selectBsInit();
	bool selectBs(int id,Instance * inst);
	bool coverUsersNew(Instance * inst);

	void resetBsScIds(Instance * inst);
	void resetCapacities(Instance * inst);
	bool generateInitialSolutionRandom(Instance * inst);

	
	void generateScSet(vector<double> & phSc, Instance * inst);
	void generateBsSet(vector<double> & phBs, double alpha, double beta, Instance * inst);
	void insertNextSc(vector<double> & phSc);
	void insertNextBs(vector<double> & phBs, double alpha, double beta, Instance * inst);
};