#pragma once
#include <vector>
#include "Instance.h"
class CplexSolver
{
public:
	long I, J, J1, J2, K, K1, K2;
	long SC_cost, BS_cost, SC_cap, BS_cap, BS_rad;
	std::vector<std::vector<long> > N;
	std::vector<std::vector<long> > D;
	std::vector<std::vector<int>> Yjk;
	long X_beg;
	long Y_beg;
	long B_beg;
	long M_beg;
	long Z_beg;
	long NUMCOLS;

	CplexSolver();
	~CplexSolver();


	void initialize(Instance * inst);

	int populatebyrow(CPXENVptr env, CPXLPptr lp);
	bool solve();
};

