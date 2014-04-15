#include <iostream>
#include "Test.h"

using namespace std;


int main(int argc, char* argv[])
{
	
	string path		= "instance/novo";
	string instName = "instExtraLarge";
	bool greedyInitialSolution = true;
	int instCount	= 2;
	int repetition	= 2;
	//srand(time(NULL));
	Test * test = new Test(path, instName, instCount, repetition);
	test->runILS(greedyInitialSolution);

	return 0;
}