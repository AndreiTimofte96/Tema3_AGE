// ati AGE_Tema3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <ctime>
#define DMAX 100
#define STOP 1
#define noOfCromosomes 100
#define MIN_VAL 9999999
#define firstVertex 1

using namespace std;

ifstream fin("data.in");

int Cost[DMAX][DMAX], Pop[DMAX][DMAX];
int noOfVertices, noOfEdges;
int localMin, globalMin = MIN_VAL;


void ReadData(){

	int x, y;
	fin >> noOfVertices >> noOfEdges;
	for (int index = 0; index < noOfEdges; index++) {
		fin >> x >> y;
		fin >> Cost[x][y];
	}
}

void InitialPopulation(){

	bool use[DMAX];
	int random, vertex;

	for (int index = 0; index < noOfCromosomes; index++) {

		for (int vertex = 2; vertex <= noOfVertices; vertex++) {
			use[vertex] = false;
		}

		Pop[index][0] = firstVertex;
		//cout << firstVertex<< " ";
		for (vertex = 1; vertex < noOfVertices; vertex++) {

			do{
				random = rand() % noOfVertices + 1;
			}
			while (use[random] == true);
			use[random] = true;
			Pop[index][vertex] = random;
			//cout << random << " ";
		}
		Pop[index][vertex] = firstVertex;
		//cout << firstVertex<< " ";
		//cout << '\n';
	}
}


int FitnessFunction(int vector[]) {

	int cost = 0;	
	for (int index = 0; index < noOfVertices; index++) {
		cost += Cost[vector[index]][vector[index + 1]];
	}
	return cost;
}

void RouletteWheel() {
	

}

void Mutation() {

}

void Cross() {

}

void GeneticAlgorithm() {

	int counter = 0;
	InitialPopulation();
	while (counter < STOP) {
		RouletteWheel();
		Mutation();
		Cross();
		//localMin = EvaluateOffSprings(functionName);
		if (localMin < globalMin) {
			counter = 0;
			globalMin = localMin;
		}
		counter++;
	}
	cout << "Best Cost: " << globalMin << '\n';
}


int main(){

	srand((unsigned int)time(NULL));
	ReadData();
	GeneticAlgorithm();
    return 0;
}

