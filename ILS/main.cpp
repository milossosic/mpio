#include <iostream>
#include "Test.h"

using namespace std;


int main(int argc, char* argv[])
{
	
	string path		= "instance/novo";
	string instName = "instExtraLarge";
	int instCount	= 20;
	int repetition	= 10;
	//srand(time(NULL));
	Test * test = new Test(path, instName, instCount, repetition);
	test->runILS(false);

	return 0;
}