#include <iostream>
#include "Test.h"
using namespace std;


int main(int argc, char* argv[])
{
	
	string path		= "instance";
	string instName =  "extraExtraLarge";
	
	int instCount	= 20;
	int repetition	= 2;

	Test * test = new Test(path, instName, instCount, repetition);
	test->runMetaheuristic(Test::ACO);
	//test->testACO();

	return 0;
}