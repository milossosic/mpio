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

void Test::runILS(int n)
{
	Config conf;
	Reader reader;
	Writer writer;
	clock_t begin_time;
	vector<Results> results;
	conf.openLog();
	
	results.resize(instCount);
	
	vector<string> instances;
	if (instName.size() > 1)
	{
		instances.push_back(instName);
	}
	else
		instances = Config::dirList(instPath);

	for (int i = 0; i < instances.size();i++)
	{
			///Solution sol;
			Instance *inst = new Instance();
			
			
			/*ostringstream ostr;
			
			ostr << instPath << "/" << instName <<  i << ".txt";
			conf.input = ostr.str();*/
			ostringstream ostr;
			ostr << instPath << "/" << instances[i];
			conf.input = ostr.str();

			reader.readInput(conf, inst);
			//conf.initialize(sol, inst);
			for (int j = 0; j < k; j++)
			{
				IteratedLocalSearch *ils = new IteratedLocalSearch();
				Solution sol = *(new Solution());
				//cout << i+1<<"."<<j+1 << endl;
				cout << conf.input << "." << j + 1 << endl;
				//conf.output << i+1 << "." << j + 1 <<" ";
				conf.outputExt << conf.input << "." << j + 1 << endl;
				conf.initialize(sol, inst);
				//conf.outputExt << i+1 << "." << j << endl;

				begin_time = clock();
				switch (n)
				{
				case OLD:	ils->runILS(sol, inst, conf); break;
				case NEW:	ils->runILSNew(sol, inst, conf); break;
				case CPLEX: ils->runILSCplex(sol, inst, conf); break;
				}
				
				//writer.printExt(conf, ils);

				results[i].time.push_back(float(clock() - begin_time) / CLOCKS_PER_SEC);
				results[i].solution.push_back(*(new Solution(ils->bestSolution)));
			}
			//ispis jednog po jednog rezultata
			//writer.printResult(conf, results[j]);
		}
		
	

	for (int j = 0; j < instances.size(); j++)
	{
		results[j].calculate();
		writer.printExtended(conf, results[j]);
	}




	conf.closeLog();
}

void Test::initialize()
{

}