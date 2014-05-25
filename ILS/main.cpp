#include <iostream>
#include "Test.h"
#include <vector>
using namespace std;


int main(int argc, char* argv[])
{
	
	string path		= "instance";
	vector<string> instNames;
	instNames.push_back("medium");
	instNames.push_back("extraLarge");
	instNames.push_back("extraExtraLarge");
	
	int instCount	= 20;
	int repetition	= 10;

	for (int i = 0; i < instNames.size(); i++)
	{
		Test * test = new Test(path, instNames[i], instCount, repetition);
		test->runMetaheuristic(Test::ILS);
		test->runMetaheuristic(Test::ACO);
		test->runMetaheuristic(Test::ACO_LS);
	}
	
	
	return 0;
}