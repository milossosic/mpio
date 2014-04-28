#include <iostream>
#include "Test.h"

using namespace std;


int main(int argc, char* argv[])
{
	
	string path		= "instance";
	string instName =  "extraLarge14_2_6_10_18_229.txt"; //	"small20_1_2_4_10_27.txt" "medium6_1_3_6_19_101.txt"
	bool greedyInitialSolution = true;
	int instCount	= 20;
	int repetition	= 5;

	Test * test = new Test(path, instName, instCount, repetition);
	test->runILS(Test::NEW);

	return 0;
}