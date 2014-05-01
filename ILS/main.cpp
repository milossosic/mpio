#include <iostream>
#include "Test.h"

using namespace std;


int main(int argc, char* argv[])
{
	
	string path		= "instance";
	string instName =  "medium";
	bool greedyInitialSolution = true;
	int instCount	= 20;
	int repetition	= 5;

	Test * test = new Test(path, instName, instCount, repetition);
	test->runMetaheuristic(Test::ILS);

	return 0;
}