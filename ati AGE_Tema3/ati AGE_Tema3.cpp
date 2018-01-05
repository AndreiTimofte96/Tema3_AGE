// ati AGE_Tema3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <ctime>
#define DMAX 1001
#define STOP 100
#define noOfCromosomes 100
#define noOfSelectedCrom noOfCromosomes/4
#define MIN_VAL 9999999.0
#define PROB_MUT 0.1

using namespace std;

ifstream fin("sahara.txt");

int firstVertex;

struct {
	int M[DMAX][DMAX];
	double CromCost[DMAX];
}Pop;

double Cost[DMAX][DMAX];

int noOfVertices, noOfEdges;
double localMin = MIN_VAL, globalMin = MIN_VAL;
int vector[DMAX];

void ReadData(){

	int x, y;
	fin >> noOfVertices >> noOfEdges;
	for (int index = 0; index < noOfEdges; index++) {
		fin >> x >> y;
		fin >> Cost[x][y];
		Cost[y][x] = Cost[x][y];
	}
}

void InitialPopulation(){

	bool use[DMAX];
	int random, vertex;

	use[firstVertex] = true;
	for (int index = 0; index < noOfCromosomes; index++) {

		for (int index1 = 1; index1 <= noOfVertices; index1++) {
			if (index1 != firstVertex) {
				use[index1] = false;
			}
		}

		Pop.M[index][0] = firstVertex;
		for (vertex = 1; vertex < noOfVertices; vertex++) {

			do{
				random = rand() % noOfVertices + 1;
			}
			while (use[random] == true);
			use[random] = true;
			Pop.M[index][vertex] = random;
		}
		Pop.M[index][vertex] = firstVertex;
	}
}


double RoadCost(int vector[]) {

	double cost = 0;	
	for (int index = 0; index < noOfVertices; index++) {
		cost += Cost[vector[index]][vector[index + 1]];
	}
	return cost;
}

void SortPopulation() {
	
	bool ok = true;
	double aux;
	int aux1;
	while (ok) {
		ok = false;
		for (int index = 0; index < noOfCromosomes - 1; index++) {
			if (Pop.CromCost[index] > Pop.CromCost[index + 1]) {
				aux = Pop.CromCost[index];
				Pop.CromCost[index] = Pop.CromCost[index+1];
				Pop.CromCost[index + 1] = aux;
				for (int vertex = 0; vertex < noOfVertices + 1; vertex++) {
					aux1 = Pop.M[index][vertex];
					Pop.M[index][vertex] = Pop.M[index+1][vertex];
					Pop.M[index + 1][vertex] = aux1;
				}
				ok = true;
			}
		}
	}
}

void RankSelection(){

	for (int index = 0; index < noOfCromosomes; index++) {
		Pop.CromCost[index] = RoadCost(Pop.M[index]);
	}

	SortPopulation();
}

void InterChange(int a[], int b[], int startPoint, int lengthOfSeq) {

	int aux[DMAX];
	vector[0] = vector[noOfVertices] = firstVertex;

	int k = 0;
	for (int index = startPoint; index < startPoint + lengthOfSeq; index++) {
		aux[k++] = a[index];
	}

	bool found;
	int final = startPoint;
	k = 1;
	for (int vertex = 1; vertex < final && vertex < startPoint + lengthOfSeq ; vertex++) {
		found = false;
		for (int index = 0; index < lengthOfSeq; index++) {
			if (b[vertex] == aux[index]) {
				found = true;
				break;
			}
		}

		if (found == false) {
			vector[k++] = b[vertex];
		}
		else {
			final++;
		}
	}
	k = 0;
	for (int vertex = startPoint; vertex < startPoint + lengthOfSeq; vertex++) {
		vector[vertex] = aux[k++];
	}

	k = startPoint + lengthOfSeq;
	for (int vertex = final; vertex < noOfVertices; vertex++) {
		found = false;
		for (int index = 0; index < lengthOfSeq; index++) {
			if (b[vertex] == aux[index]) {
				found = true;
				break;
			}
		}
		if (found == false) {
			vector[k++] = b[vertex];
		}
	}
}

void Cross(){

	int selectCrom = noOfCromosomes - noOfSelectedCrom - 1;
	int lengthOfSeq = noOfVertices / 2;
	int startPoint = rand() % (noOfVertices - lengthOfSeq) + 1;
	int newB[DMAX], newA[DMAX];
	if (selectCrom % 2 == 1) selectCrom++;

	for (int index = selectCrom; index < noOfCromosomes; index += 2) {
		
		InterChange(Pop.M[index], Pop.M[index + 1], startPoint, lengthOfSeq);
		for (int index1 = 0; index1 < noOfVertices + 1; index1++) {
			newB[index1] = vector[index1];
		}
		InterChange(Pop.M[index + 1], Pop.M[index], startPoint, lengthOfSeq);
		for (int index1 = 0; index1 < noOfVertices + 1; index1++) {
			newA[index1] = vector[index1];
		}
		

		for (int index1 = 0; index1 < noOfVertices + 1; index1++) {
			Pop.M[index][index1] = newA[index1];
			Pop.M[index + 1][index1] = newB[index1];
		}
	}
}

double RandomValue(double lowValue, double highValue) {
	return lowValue + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (highValue - lowValue)));
}

void Mutation() {
	
	int random1, random2, aux;
	double prob;
	for (int cromosome = 0; cromosome < noOfCromosomes; cromosome++) {

		prob = RandomValue(0, 1);
		if (prob < PROB_MUT) {

			do {
				random1 = rand() % (noOfVertices - 1) + 1;
				random2 = rand() % (noOfVertices - 1) + 1;
			} while (random1 == random2);

			aux = Pop.M[cromosome][random1];
			Pop.M[cromosome][random1] = Pop.M[cromosome][random2];
			Pop.M[cromosome][random2] = aux;
		}
		
	}

}

void GeneticAlgorithm() {

	int counter = 0;
	InitialPopulation();
	while (counter < STOP) {
		RankSelection();
		Cross();
		Mutation();
		localMin = Pop.CromCost[0];
		if (localMin < globalMin) {
			counter = 0;
			globalMin = localMin;
			cout << localMin << '\n';
		}
		counter++;
	}
	cout << "Best Cost: " << globalMin << '\n';
	for (int index = 0; index < noOfVertices + 1; index++) {
		cout << Pop.M[0][index] << " ";
	}
	cout << '\n';
}

int main(){

	double allMin = MIN_VAL;
	srand((unsigned int)time(NULL));
	ReadData();
	for (firstVertex = 1; firstVertex <= 38; firstVertex++) {
		globalMin = MIN_VAL;
		
		cout << "Start Vertex: " << firstVertex << '\n';
		GeneticAlgorithm();
		if (globalMin < allMin) allMin = globalMin;
	}

	cout << "Best MIn Cost: " << allMin<<'\n';


    return 0;
}



