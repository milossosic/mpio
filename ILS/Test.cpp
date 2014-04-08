#include <sstream>
#include <iostream>
#include <ctime>
#include "IteratedLocalSearch.h"
#include "Reader.h"
#include "Writer.h"
#include "Solution.h"
#include "Config.h"
#include "Instance.h"
#include "Test.h"
#include "Results.h"

Test::Test()
{
}

Test::Test(string & path, string & name, int count, int _k) :
 instCount(count), k(_k)
{
	instPath = path;
	instName = name;
}
Test::~Test()
{
}

void Test::run()
{
	Config conf;
	Reader reader;
	Writer writer;
	clock_t begin_time;
	vector<Results> results;
	conf.openLog();
	
	results.resize(instCount);
	
	

		for (int i = 1; i <= instCount; i++)
		{
			Solution sol;
			Instance *inst = new Instance();
			
			
			ostringstream ostr;
			ostr << instPath << "/" << instName <<  i << ".txt";
			conf.input = ostr.str();

			reader.readInput(conf, inst);
			//conf.initialize(sol, inst);
			for (int j = 0; j < k; j++)
			{
				IteratedLocalSearch *ils = new IteratedLocalSearch();
				Solution sol;
				
				conf.initialize(sol, inst);
				begin_time = clock();
				conf.outputExt << i << "."<< j << endl;
				ils->runILS(sol, inst, conf);

				//writer.printExt(conf, ils);

				results[i - 1].time.push_back(float(clock() - begin_time) / CLOCKS_PER_SEC);
				results[i - 1].solution.push_back(*(new Solution(ils->bestSolution)));
			}
			//ispis jednog po jednog rezultata
			//writer.printResult(conf, results[j]);
		}
		
	

	for (int j = 0; j < instCount; j++)
	{
		results[j].calculate();
		writer.printExtended(conf, results[j]);
	}




	conf.closeLog();
}

void Test::initialize()
{

}