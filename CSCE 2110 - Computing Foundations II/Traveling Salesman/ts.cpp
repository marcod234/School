/*-------------------------------------------------------------------*\
|                             Marco Duarte                            |
|                        marcoduarte2@my.unt.edu                      |
|                            CSCE 2110:001                            |
|                            March 24, 2016                            |
|                                                                     |
|    This program calculates the cheapest tour from one city,         |
|    visiting every other city, and returning to the original.        |
|    Input is taken from stdin for a list of 'n' cities each with     |
|    'n' amount of prices to each city.                               |
|                                                                     |
\*-------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

void getCost(int* list, int** arcs, string &Path, int &cost, int citynum);
void cellSwitch(int* list, int x, int y);
void path(int* list, int start, int citynum, int** arcs, string &Path, int &cost);
void print_arcs(int** arcs, int citynum);

int main()
{
	char in = 'c';
	int city, city2, cost=0, citynum = 0; // city and city2 are used as indexes
	int* list;
	int** arcs;
	string temps, Path;

	while (cin >> in)
	{
		if (in != 'c')
			break;
		cin >> city;
		citynum++;
	}

	arcs = new int*[citynum]; //allocate space
	list = new int[citynum];
	for (int i = 0; i < citynum; i++)
	{
		arcs[i] = new int[citynum];
		list[i] = i; //assigns city numbers
	}

	do
	{
		if (in == 'a')//reading in and assigning city costs
		{
			cin >> city >> city2 >> cost;
			arcs[city - 1][city2 - 1] = cost;
			arcs[city2 - 1][city - 1] = cost;
		}
	} while (cin >> in);
	cost = 0; //reset cost
	path(list, 1, citynum - 1, arcs, Path, cost); 
	cout << Path << " costs " << cost << endl;

	return 0;
}

void getCost(int* list, int** arcs, string &Path, int &cost, int citynum)
{
	int sum = 0;
	stringstream sstm;
	string temp = "The tour ";
	for (int i = 0; i <= citynum; i++)
	{
		sstm.str(string());
		if(i == citynum)//last city
			sum+= arcs[list[citynum]][list[0]];//gets cost from that city
		else
			sum += arcs[ list[i] ][ list[i + 1] ];

		if(sum > cost && cost != 0)//exit as soon as sum > cost
		  return;

		sstm << list[i]+1;
		temp += sstm.str() + " "; //adds city number to string
	}
	sstm.str(string());
	sstm << list[0]+1;
	temp += sstm.str();

	cost = sum;
	Path = temp;
}

void cellSwitch(int* list, int x, int y) //swaps city number
{
	int temp;
	temp = list[x];
	list[x] = list[y];
	list[y] = temp;
}

void path(int* list, int start, int citynum, int** arcs, string &Path, int &cost)
{
	if (start == citynum) //ending position
	{
		getCost(list, arcs, Path, cost, citynum);
	}

	for (int i = start; i <= citynum; i++)//generate all sequences recursiveley
	{
		cellSwitch(list, start, i);
		path(list, start + 1, citynum, arcs, Path, cost);
		cellSwitch(list, start, i);
	}
}

void print_arcs(int** arcs, int citynum)
{
	for (int i = 0; i < citynum; i++)
	{
		for (int j = 0; j < citynum; j++)
			cout << setw(5) << arcs[i][j];
		cout << endl;
	}
	
	cout << endl << endl;
}
