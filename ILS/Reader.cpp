#include <fstream> 
#include "Reader.h"
#include "Instance.h"
#include "Config.h"

Reader::Reader()
{
}


Reader::~Reader()
{
}

void Reader::readInput(Config & c, Instance &inst)
{
	fstream fin;
	fin.open(c.input, ifstream::in);

	fin >> inst.scOldCount >> inst.scNewCount >> inst.bsOldCount >> inst.bsNewCount >> inst.usCount;
	fin >> inst.scCost >> inst.bsCost >> inst.scCapacity >> inst.bsCapacity;
	fin >> inst.bsRadius;
	for (int i = 0; i < inst.bsOldCount; i++)
		for (int j = 0; j < inst.scOldCount; j++)
		{
			fin >> inst.bsScConnMatrix[i][j];
		}

	for (int i = 0; i < inst.bsNewCount; i++)
		for (int j = 0; j < inst.scOldCount + inst.scNewCount; j++)
		{
			fin >> inst.bsScConnCost[i][j];
		}

	for (int i = 0; i < inst.usCount; i++)
	{

		for (int j = 0; j < inst.bsNewCount + inst.bsOldCount; j++)
		{
			fin >> inst.usBsRadius[i][j];
		}
	}

	fin.close();

}
