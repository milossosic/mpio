#include <iostream>
#include <cstdlib>
#include <time.h>
#include "Test.h"

using namespace std;


int main(int argc, char* argv[])
{
	
	string path		= "instance";
	string instName = "instLarge";
	int instCount	= 1;
	int repetition	= 20;
	//srand(time(NULL));
	Test * test = new Test(path, instName, instCount, repetition);
	test->run();

	return 0;
}