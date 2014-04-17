#pragma once
#include <queue>
#include <utility>
#include <cstdlib>
#include "Instance.h"
#include "switchingCenter.h"
#include "baseStation.h"
#include "CplexSolver.h"
using namespace std;
class Solution
{
public:
	int minLsCost;
	int bestCost;
	int currentCost;
	bool greedyConn;
	
	vector<int> M, B;
	deque<int> scSet;
	deque<pair<int, int>> bsSet;
	int bsFixed;
	
	deque<baseStation> originalBaseStations;
	deque<baseStation> baseStations;
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

	//deque<pair<int, int>> bsScConnList;

	Solution();
	Solution(const Solution & sol);
	~Solution();

	void insertRealBs(int id, int scId);


	void initialize(Instance * inst);

	void bsIdInvert();
	int insertBs(int id, int scId);
	void insertRealBsCplex(int id, Instance *inst);
	void removeRealBsCplex(int id, Instance *inst);
	int insertBsCplex(int id, Instance * inst);
	int removeBs(int id);
	int removeBsCplex(int id,Instance *inst);
	// id je redni broj sc-a u currentSwitchingCenters
	void insertSc(int id);

	// id - redni broj sc-a u scSet-u
	void removeSc(int id);

	int insertRandomBs(Instance * inst, bool greedy);

	int removeRandomBs();

	int insertRandomSc();

	int removeRandomSc();

	void setGreedyConn(Instance *inst, int bsId);

	void resetBs(Instance * inst);

	void resetBsCplex(Instance * inst);

	void resetSolution(Instance * inst);

	void resetSolutionCplex(Instance * inst);

	void setRandomScConn(int id, Instance * inst, bool greedy);

	void genInitScSet(Instance * inst);

	void genInitScSetCplex(Instance * inst);

	int totalCost(Instance * inst);

	int totalCostCplex(Instance * inst, CplexSolver * cpl);

	void generateBsMustSet(Instance * inst);

	void generateBsMustSetCplex(Instance * inst);

	void genInitBsSet(Instance * inst, int bsN);

	void genInitBsSetCplex(Instance * inst, int bsN);

	bool coverUsers(Instance * inst);

	void resetBsScIds(Instance * inst);

	void resetCapacities(Instance * inst);


	void generateInitialSolutionRandom(Instance * inst);

	void generateInitialSolutionCplex(Instance * inst, CplexSolver * cpl);

	void generateInitialSolutionGreedy(Instance * inst);
};