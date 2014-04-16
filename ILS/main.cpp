#include <iostream>
#include "Test.h"

using namespace std;


int main(int argc, char* argv[])
{
	
	string path		= "instance";
	string instName = "instLarge";
	bool greedyInitialSolution = true;
	int instCount	= 20;
	int repetition	= 5;
	//srand(time(NULL));
	Test * test = new Test(path, instName, instCount, repetition);
	test->runILS(greedyInitialSolution);

	return 0;
}