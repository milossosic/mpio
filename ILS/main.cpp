#include <iostream>
#include "Test.h"
using namespace std;


int main(int argc, char* argv[])
{
	
	string path		= "instance";
	string instName =  "medium";
	
	int instCount	= 20;
	int repetition	= 1;

	Test * test = new Test(path, instName, instCount, repetition);
	test->runMetaheuristic(Test::ACO);
	
	return 0;
}