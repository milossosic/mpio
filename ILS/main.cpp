#include <iostream>
#include "Test.h"

using namespace std;


int main(int argc, char* argv[])
{
	
	string path		= "instance";
	string instName = "instSmall";
	bool greedyInitialSolution = true;
	int instCount	= 10;
	int repetition	= 5;

	Test * test = new Test(path, instName, instCount, repetition);
	test->runILS(Test::CPLEX);

	return 0;
}