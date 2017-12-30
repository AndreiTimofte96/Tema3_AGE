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
#define MIN_VAL 9999999
#define firstVertex 1

using namespace std;

ifstream fin("data.in");

struct {
	int M[DMAX][DMAX];
	int CromCost[DMAX];
}Pop;

int Cost[DMAX][DMAX];

int noOfVertices, noOfEdges;
int localMin = MIN_VAL, globalMin = MIN_VAL;
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
		cout << firstVertex << " ";
		for (vertex = 1; vertex < noOfVertices; vertex++) {

			do{
				random = rand() % noOfVertices + 1;
			}
			while (use[random] == true);
			use[random] = true;
			Pop.M[index][vertex] = random;
			cout << random << " ";
		}
		Pop.M[index][vertex] = firstVertex;
		cout << firstVertex<< " ";
		cout << '\n';
	}
}


int RoadCost(int vector[]) {

	int cost = 0;	
	for (int index = 0; index < noOfVertices; index++) {
		cost += Cost[vector[index]][vector[index + 1]];
	}
	return cost;
}

void SortPopulation() {
	
	bool ok = true;
	int aux;
	while (ok) {
		ok = false;
		for (int index = 0; index < noOfCromosomes - 1; index++) {
			if (Pop.CromCost[index] > Pop.CromCost[index + 1]) {
				aux = Pop.CromCost[index];
				Pop.CromCost[index] = Pop.CromCost[index+1];
				Pop.CromCost[index + 1] = aux;
				for (int vertex = 0; vertex < noOfVertices + 1; vertex++) {
					aux = Pop.M[index][vertex];
					Pop.M[index][vertex] = Pop.M[index+1][vertex];
					Pop.M[index + 1][vertex] = aux;
				}
				ok = true;
			}
		}
	}
}

void RankSelection(){
	cout << "Costuri Crom:\n";
	for (int index = 0; index < noOfCromosomes; index++) {
		Pop.CromCost[index] = RoadCost(Pop.M[index]);
		cout << Pop.CromCost[index] << " ";
	}
	cout << '\n';
	
	/*for (int i = 0; i < noOfCromosomes; i++) {
		for (int j = 0; j < noOfVertices + 1; j++) {
			cout << Pop.M[i][j] << " ";
		}
		cout <<"Cost " << Pop.CromCost[i] << '\n';
	}*/
	SortPopulation();
}

void InterChange(int a[], int b[], int startPoint, int lengthOfSeq) {

	int aux[DMAX];
	int  k = 0;
	bool found;

	for (int index = startPoint; index < startPoint + lengthOfSeq; index++) {
		aux[k++] = a[index];
	}

	int fin = startPoint;
	k = 0;
	for (int index = 0; index < fin && index < startPoint+lengthOfSeq; index++) {
		found = false;
		for (int seq = 0; seq < lengthOfSeq; seq++) {
			if (b[index] == aux[seq]) {
				found = true;
			}
		}
		if (found == false) {
			vector[k++] = b[index];
		}
		else {
			fin++;
		}
	}

	k = 0;
	for (int index = startPoint; index < startPoint + lengthOfSeq; index++) {
		vector[index] = aux[k++];
	}

	k = startPoint + lengthOfSeq;
	for (int index = startPoint; index < noOfVertices; index++) {
		found = false;
		for (int seq = 0; seq < lengthOfSeq; seq++) {
			if (b[index] == aux[seq]) {
				found = true;
			}
		}
		if (found == false) {
			vector[k++] = b[index];
		}	
	}
	vector[noOfVertices] = firstVertex;
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
		
		/*for (int index1 = 0; index1 < noOfVertices + 1; index1++) {
			cout << Pop.M[index][index1] << " ";
		}
		cout << "=> ";
		for (int index1 = 0; index1 < noOfVertices + 1; index1++) {
			cout << newA[index1] << " ";
		}
		cout << '\n';
		for (int index1 = 0; index1 < noOfVertices + 1; index1++) {
			cout << Pop.M[index+1][index1] << " ";
		}
		cout << "=> ";
		for (int index1 = 0; index1 < noOfVertices + 1; index1++) {
			cout << newB[index1] << " ";
		}*/


		for (int index1 = 0; index1 < noOfVertices + 1; index1++) {
			Pop.M[index][index1] = newA[index1];
			Pop.M[index + 1][index1] = newB[index1];
		}
		/*cout << '\n';
		for (int index1 = 0; index1 < noOfVertices + 1; index1++) {
			cout << Pop.M[index][index1] << " ";
		}
		cout << '\n';
		for (int index1 = 0; index1 < noOfVertices + 1; index1++) {
			cout << Pop.M[index+1][index1] << " ";
		}
		cout << '\n';
		cout << startPoint << " " << lengthOfSeq << '\n';*/
	}
}


void Mutation() {

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
		}
		counter++;
	}
	cout << "Best Cost: " << globalMin << '\n';
	for (int index = 0; index < noOfVertices + 1; index++) {
		cout << Pop.M[0][index] << " ";
	}
}



int main(){

	srand((unsigned int)time(NULL));
	ReadData();
	GeneticAlgorithm();

	/*int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	int b[] = { 7, 3, 1, 11, 4, 12, 5, 2, 10, 9, 6, 8 }; */

	cout<<'\n';
	int a[] = { 1, 5, 2, 4, 6, 3, 1 };
	int b[] = { 1, 3, 6, 2, 5, 4, 1 };
	int length = 7;
	int newA[DMAX], newB[DMAX];
	
	InterChange(a, b, 3, 3);
	for (int index = 0; index < length; index++) {
		newA[index] = vector[index];
	}
	
	InterChange(b, a, 3, 3);
	for (int index = 0; index < length; index++) {
		newB[index] = vector[index];
	}

	for (int index = 0; index < length; index++) {
		cout << newA[index] << " ";
	}
	cout << '\n';
	for (int index = 0; index < length; index++) {
		cout << newB[index] << " ";
	}
    return 0;
}

