#include "Ant.h"


Ant::Ant()
{
}


Ant::~Ant()
{
}

void Ant::runAnt(vector<double> &phBs, vector<double> &phSc, double alpha, double beta, Instance * inst)
{
	s.resetSolution(inst);

	
	s.generateScSet(phSc, inst);
	for (int j = 0; j < s.bsFixed; j++)
	{
		s.setRandomScConn(j, inst);
	}
	s.generateBsSet(phBs, alpha, beta, inst);

}