#include <iostream>
#include <random>
#include <fstream>
#include <ctime>
#include <cmath>
#include <sstream>
using namespace std;


const int M = 1000;

int no_inst = 20;

const int SC_old_num = 2;
const int SC_new_num = 5;
const int BS_old_num = 8;
const int BS_new_num = 20;
const int point_num = 230;

int SC_num = SC_old_num + SC_new_num;
int BS_num = BS_old_num + BS_new_num;

const int SC_cost = 100000;
const int BS_cost = 20000;
const int BS_SC_cost = 40;

const int SC_capacity = 100;
const int BS_capacity = 40;

const int SC_min_radius = 150;
const int BS_max_radius = 300;
const int BS_min_radius = 40;

struct base_station;
struct switching_center;

struct point
{
public:
	int x, y;
	point()
	{};
	point(int _x, int _y) :
		x(_x), y(_y)
	{};
	int distance(base_station &);
	int distance(switching_center &);
	bool inRadius(base_station*);
};

struct base_station : point
{
public:
	base_station()
	{};
	base_station(int _x, int _y):
		point(_x,_y)
	{};
	bool correctPlacement(base_station *bs, int t1)
	{
		for (int i = 0; i < t1; i++)
		{
			if (distance(bs[i]) < 2 * BS_min_radius)
				return false;
		}
		return true;
	}
};

struct switching_center : point
{};

int point::distance(base_station &b)
{
	return static_cast<int>(sqrt(pow(x - b.x, 2) + pow(y - b.y, 2)));
};

int point::distance(switching_center &b)
{
	return static_cast<int>(sqrt(pow(x - b.x, 2) + pow(y - b.y, 2)));
};

bool point::inRadius(base_station *bs)
{
	for (int i = 0; i < BS_num; i++)
	{
		if (distance(bs[i]) < BS_max_radius)
			return true;
	}
	
	return false;
};

int main()
{
	for (int no = 1; no <= no_inst; no++)
	{
		ostringstream ostr;
		ostr << "instSmall" << no << ".txt";
		
		ofstream out(ostr.str());



		point demand_points[point_num];
		switching_center switching_centers[SC_old_num + SC_new_num];
		base_station base_stations[BS_old_num + BS_new_num];




		//int rand_seed = 1;
		//srand(rand_seed);

		default_random_engine generator;
		generator.seed(no);
		uniform_int_distribution<int> dist(0, M);

		out << SC_old_num << endl;
		out << SC_new_num << endl;
		out << BS_old_num << endl;
		out << BS_new_num << endl;
		out << point_num << endl;
		out << SC_cost << endl;
		out << BS_cost << endl;
		out << SC_capacity << endl;
		out << BS_capacity << endl;
		out << BS_max_radius << endl;
		//switching centers generation
		for (int i = 0; i < SC_num; i++)
		{
			switching_centers[i].x = dist(generator);
			switching_centers[i].y = dist(generator);
		}



		//base stations generation
		int j = 0;
		long min;
		long temp;
		int k_min;
		vector<vector<int>> Yjk;
		Yjk.resize(BS_old_num);
		for (int j = 0; j < BS_old_num; j++)
		{
			Yjk[j].resize(SC_old_num);
		}
		
		while (j < BS_old_num)
		{
			base_stations[j].x = dist(generator);
			base_stations[j].y = dist(generator);
			if (!base_stations[j].correctPlacement(base_stations, j))
				continue;
			min = INT_MAX;
			for (int k = 0; k < SC_old_num; k++)
			{
				temp = base_stations[j].distance(switching_centers[k]) * BS_SC_cost;
				if (temp < min)
				{
					min = temp;
					k_min = k;
				}
				
			}
			Yjk[j][k_min] = 1;
			
			j++;
		}
		for (int j = 0; j < BS_old_num; j++)
		{
			for (int k = 0; k < SC_old_num; k++)
				out << Yjk[j][k] << " ";
			out << endl;
		}

		while (j < BS_num)
		{
			base_stations[j].x = dist(generator);
			base_stations[j].y = dist(generator);
			if (!base_stations[j].correctPlacement(base_stations, j))
				continue;
			
			for (int k = 0; k < SC_num - 1; k++)
			{
				out << base_stations[j].distance(switching_centers[k]) * BS_SC_cost << " ";
			}
			out << (base_stations[j].distance(switching_centers[SC_num - 1]) * BS_SC_cost)  << endl;

			j++;
		}


		//demand points generation
		int i = 0;
		
		while (i < point_num)
		{
			demand_points[i].x = dist(generator);
			demand_points[i].y = dist(generator);
			if (!demand_points[i].inRadius(base_stations))
				continue;
			
			for (int j = 0; j < BS_num - 1; j++)
			{
				out << demand_points[i].distance(base_stations[j]) << " ";
			}
			out << demand_points[i].distance(base_stations[BS_num - 1]) << endl;
			i++;
		}
		
		out.close();


	}
}