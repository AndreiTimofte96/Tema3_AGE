// ati AGE_Tema3.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <ctime>
#define DMAX 1001
#define STOP 100
#define noOfCromosomes 1000
#define MIN_VAL 9999999.0
#define MAX_VAL 0.0
#define PROB_MUT 0.01
#define PROB_CROSS 0.2

using namespace std;

ifstream fin("sahara.txt");

int firstVertex;

struct {
	int M[DMAX][DMAX];
	int newM[DMAX][DMAX];
	double CromCost[DMAX];
}Pop;

double Cost[DMAX][DMAX];

int noOfVertices, noOfEdges;
double bestChromosome = MAX_VAL, bestResult = MIN_VAL;
int posOfBest;
int vector[DMAX], optimTour[DMAX];

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

double FitnessFunction(double value) {

	return 1 / value;
}

void CopyFromMatrix(int survivors[]) {

	for (int index = 0; index < noOfCromosomes; index++) {
		for (int vertex = 0; vertex < noOfVertices + 1; vertex++) {
			Pop.newM[index][vertex] = Pop.M[survivors[index]][vertex];
		}
	}
}

void CopyToMatrix() {

	for (int index = 0; index < noOfCromosomes; index++) {
		for (int vertex = 0; vertex < noOfVertices + 1; vertex++) {
			Pop.M[index][vertex] = Pop.newM[index][vertex];
		}
	}
}

double RandomValue(double lowValue, double highValue) {
	return lowValue + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (highValue - lowValue)));
}

void RouletteSelection() {

	double result, cromosomeSum = 0, random;
	double prob[noOfCromosomes + 1];
	double qprob[noOfCromosomes + 1];

	int survivorsChrom[noOfCromosomes];

	for (int index = 0; index < noOfCromosomes; index++) {

		result = RoadCost(Pop.M[index]);
		if (result < bestResult) {
			bestResult = result;
			posOfBest = index;
			cout << bestResult << '\n';
		}
		Pop.CromCost[index] = FitnessFunction(result);
	}

	for (int index = 0; index < noOfCromosomes; index++) {
		cromosomeSum += Pop.CromCost[index];
		if (Pop.CromCost[index] > bestChromosome) {
			bestChromosome = Pop.CromCost[index];
		}
	}

	for (int index1 = 0; index1 < noOfCromosomes; index1++) {
		prob[index1] = Pop.CromCost[index1] / cromosomeSum;
	}

	qprob[0] = 0;
	for (int index1 = 0; index1 < noOfCromosomes; index1++) {
		qprob[index1 + 1] = qprob[index1] + prob[index1];
	}

	for (int index = 0; index < noOfCromosomes; index++) {
		random = RandomValue(0, 1);
		survivorsChrom[index] = 0;
		for (int index1 = 0; index1 < noOfCromosomes; index1++)
			if (qprob[index1] <= random && random <= qprob[index1 + 1]) {
				survivorsChrom[index] = index1;
			}
	}
	CopyFromMatrix(survivorsChrom);
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

	int lengthOfSeq = noOfVertices / 2;
	int startPoint = rand() % (noOfVertices - lengthOfSeq) + 1;
	int newB[DMAX], newA[DMAX];

	double random;
	int crossSurv[noOfCromosomes];
	int k = 0;
	for (int index = 0; index < noOfCromosomes; index++) {
		random = RandomValue(0, 1);
		if (random < PROB_CROSS) {
			crossSurv[k++] = index;
		}
	}

	if (k % 2 == 1) {
		k--;
	}

	for (int index = 0; index < k-1; index += 2) {
		
		InterChange(Pop.newM[crossSurv[index]], Pop.newM[crossSurv[index + 1]], startPoint, lengthOfSeq);
		for (int index1 = 0; index1 < noOfVertices + 1; index1++) {
			newB[index1] = vector[index1];
		}
		InterChange(Pop.newM[crossSurv[index + 1]], Pop.newM[crossSurv[index]], startPoint, lengthOfSeq);
		for (int index1 = 0; index1 < noOfVertices + 1; index1++) {
			newA[index1] = vector[index1];
		}
		
		for (int index1 = 0; index1 < noOfVertices + 1; index1++) {
			Pop.newM[crossSurv[index]][index1] = newA[index1];
			Pop.newM[crossSurv[index + 1]][index1] = newB[index1];
		}
	}
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

			aux = Pop.newM[cromosome][random1];
			Pop.newM[cromosome][random1] = Pop.newM[cromosome][random2];
			Pop.newM[cromosome][random2] = aux;
		}
	}
}

double EvaluateOffSprings() {

	double best = MAX_VAL, result;

	for (int index = 0; index < noOfCromosomes; index++) {

		result = FitnessFunction(RoadCost(Pop.newM[index]));
		if (result > best) {
			best = result;
		}
	}
	return best;
}

void GeneticAlgorithm() {

	int counter = 0;
	double result;
	InitialPopulation();
	while (counter < STOP) {
		//RankSelection();
		RouletteSelection();
		Cross();
		Mutation();
		result = EvaluateOffSprings();
		if (result > bestChromosome) {
			counter = 0;
			bestChromosome = result;
		}
		CopyToMatrix();
		counter++;
	}
	cout << "Best Cost: " << bestResult << '\n';
	for (int index = 0; index < noOfVertices + 1; index++) {
		cout << Pop.M[posOfBest][index] << " ";
	}
	cout << '\n';
}

int main(){

	double allMin = MIN_VAL;
	srand((unsigned int)time(NULL));
	ReadData();

	//int x = noOfVertices;
	int x = 1;
	
	for (firstVertex = 1; firstVertex <= x; firstVertex++) {
		bestResult = MIN_VAL;
		bestChromosome = MAX_VAL;
		
		cout << "Start Vertex: " << firstVertex << '\n';
		GeneticAlgorithm();
		if (bestResult < allMin) {
			allMin = bestResult;
			for (int index = 0; index < noOfVertices + 1; index++) {
				optimTour[index] = Pop.M[posOfBest][index];
			}
		}

	}
	cout << "Best Min Cost: " << allMin << '\n';
	for (int index = 0; index < noOfVertices + 1; index++) {
		cout << optimTour[index] << " ";
	}
	cout << '\n';
    return 0;
}

