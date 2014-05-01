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
	s.generateBsSet(phBs, alpha, beta, inst);

}