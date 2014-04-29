#include <ctime>
#include <vector>
#include <windows.h>
#include <tchar.h>
#include <cstdlib>
#include <random>
#include "Config.h"
#include "Solution.h"
#include "Instance.h"


std::default_random_engine Config::generator(time(NULL));

Config::Config()
{
	//output.open("log.txt");
}


Config::~Config()
{
}

void Config::openLog()
{
	output.open("log.txt");
	outputExt.open("logExt.txt");
}

void Config::closeLog()
{
	output.close();
	outputExt.close();
}

bool Config::comparePairsAsc(const pair<int, int> &a, const pair<int, int> &b)
{
	return a.second < b.second;
}

bool Config::comparePairsDsc(const pair<int, int> &a, const pair<int, int> &b)
{
	return a.second > b.second;
}


void Config::initialize(Solution & s, Instance * inst)
{
	s.originalBaseStations.resize(inst->bsOldCount + inst->bsNewCount);
	inst->users.clear();
	inst->users.resize(inst->usCount);

	for (int i = 0; i < inst->usCount; i++)
	{
		for (int j = 0; j < inst->bsOldCount + inst->bsNewCount; j++)
		{
			int rad = inst->usBsRadius[i][j];
			if (rad < inst->bsRadius)
			{
				inst->users[i].bsSet.push_back(make_pair(j, rad));
				s.originalBaseStations[j].users.insert(i);
			}
		}
	}
	inst->initialize();
	s.initialize(inst);
}

int Config::Rand()
{
	uniform_int_distribution<int> distribution(1, 2000202);
	return distribution(generator);
}

vector<string> Config::dirList(string dir, string instName)
{
	vector<string> ret;
	WIN32_FIND_DATA ffd;
	wchar_t path[100];
	int i = 0;
	do
	{
		path[i] = dir.c_str()[i];
	} while (dir.c_str()[i++] != 0);
	
	if (path[_tcslen(path) - 1] != '\\')
		_tcscat(path, _T("\\"));
	_tcscat(path, _T("*.*"));

	HANDLE hFind = FindFirstFile(path, &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		//cerr << _T("Invalid handle value.") << GetLastError() << endl;
		return ret;
	}

	bool finished = false;
	while (!finished)
	{
		//cout << ffd.cFileName << endl;
		char name[50];
		int i = 0;
		do
		{
			name[i] = ffd.cFileName[i];
		} while (ffd.cFileName[i++] != 0);
		if (instName.length() > 0)
		{
			for (int i = 0; i < instName.length(); i++)
			{
				if (instName[i] != name[i])
					continue;
			}
		}
		ret.push_back(name);
		if (ret[ret.size() - 1][0] == '.')
			ret.erase(ret.begin() + ret.size()-1);

		if (!FindNextFile(hFind, &ffd))
			finished = true;
	}




	return ret;
}