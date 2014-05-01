#include <iostream>
#include "Test.h"
#include "Config.h"
using namespace std;


int main(int argc, char* argv[])
{
	
	string path		= "instance";
	string instName =  "small";
	bool greedyInitialSolution = true;
	int instCount	= 20;
	int repetition	= 5;

	Test * test = new Test(path, instName, instCount, repetition);
	test->runMetaheuristic(Test::ACO);
	
	
	int a[10];
	for (int i = 0; i < 10; i++)
		a[i] = 0;
	for (int i = 0; i < 10000; i++)
	{
		a[(int)(Config::RandDouble() * 10)]++;
	}
	for (int i = 0; i < 10; i++)
		cout << a[i] << endl;

	return 0;
}