#include <iostream>
#include "Test.h"

using namespace std;


int main(int argc, char* argv[])
{
	
	string path		= "instance";
	string instName =  ""; //	"small20_1_2_4_10_27.txt" "medium6_1_3_6_19_101.txt"
	bool greedyInitialSolution = true;
	int instCount	= 20;
	int repetition	= 10;

	Test * test = new Test(path, instName, instCount, repetition);
	test->runILS(Test::NEW);

	return 0;
}