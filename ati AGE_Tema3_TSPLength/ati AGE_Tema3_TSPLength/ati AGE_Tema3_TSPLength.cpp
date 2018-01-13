// ati AGE_Tema3_TSPLength.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <math.h>
#define DMAX 10009
#define noOfCities 101

using namespace std;

ifstream fin("ei;.in");
ofstream fout("eil.txt");

struct {
	double x[DMAX];
	double y[DMAX];
	double dist[DMAX][DMAX];
}p;

double distanceCalculate(double x1, double y1, double x2, double y2)
{
	double x = x1 - x2;
	double y = y1 - y2;
	double dist;
	dist = pow(x, 2) + pow(y, 2);          
	dist = sqrt(dist);                  
	return dist;
}

int main()
{
	int nr;
	double x, y;

	for (int index = 1; index <= noOfCities; index++) {

		fin >> nr >> p.x[index] >> p.y[index];
	}
	int noOfEdges = 0;
	for (int index = 1; index <= noOfCities; index++) {
		for (int jindex = index + 1; jindex <= noOfCities; jindex++) {
			p.dist[index][jindex] = p.dist[jindex][index] =
			distanceCalculate(p.x[index], p.y[index], p.x[jindex], p.y[jindex]);
			noOfEdges++;
		}
	}

	fout << noOfCities << " " << noOfEdges<<'\n';
	
	for (int index = 1; index <= noOfCities; index++) {
		for (int jindex = index+1; jindex <= noOfCities; jindex++) {
			fout << index<< " "<<jindex<< " "<<p.dist[index][jindex] << '\n';

		}
	}
	
    return 0;
}

