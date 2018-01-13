// t3 comis voiajor euristic.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#define MAXNUM 10000 /* numar maxim orase */
#define MINIMUM 1000000 /* initializare valoare minim */
#define FILENAME "berlin.txt" /* nume fisier date */

int n; /* numar orase */
double d[MAXNUM][MAXNUM]; /* matricea cu distantele dintre orase */
int path[MAXNUM]; /* calea comisului voiajor ce contine orasele vizitate */
bool visited[MAXNUM]; /* verifica daca un oras a fost viziatat sau nu */

void choose_best(int last, double &min_dist, int &pos_min);

int main(void) {
	FILE *fin;
	int x, y, m;
	int i, j, crt_city, count, best_path[MAXNUM], pos_min;
	double min, crt_cost, best_cost = MINIMUM;
	fin = fopen(FILENAME, "r");
	if (!fin) {
		printf("Eroare deschidere fisier.\n");
		return -1;
	}

	fscanf(fin, "%d %d", &n, &m); /* n - nr orase, m - nr legaturi dintre orase */

	for (j = 0; j < m; j++) {
		fscanf(fin, "%d %d", &x, &y);
		fscanf(fin, "%lf", &d[x][y]);
		d[y][x] = d[x][y];
	}

	/*printf("%d orase.\n", n);
	printf("Distanta dintre orase:\n");
	for (i = 1; i <= n; i++) {
	for (j = 1; j <= n; j++)
	printf("%f ", d[i][j]);
	printf("\n");
	}
	printf("\n");
	*/

	for (crt_city = 1; crt_city <= n; crt_city++) {
		printf("Oras de start: %d\n", crt_city);
		min = MINIMUM;
		pos_min = MINIMUM;
		for (i = 1; i <= n; i++) {
			visited[i] = 0; /* niciun oras nu a fost vizitat */
			path[i] = 0;
		}
		/* initializare primul oras vizitat */
		path[1] = crt_city; visited[crt_city] = 1;
		count = 2; crt_cost = 0;

		/* vizitam restul oraselor in mod greedy */
		for (i = 1; i <= n - 1; i++) {
			choose_best(path[count - 1], min, pos_min);
			printf("Am adaugat drumul (%d, %d) de cost %f.\n", path[count - 1], pos_min, min);
			path[count] = pos_min;
			visited[pos_min] = 1;
			count++;
			crt_cost += min;
		}
		/* calculam si distanta de la ultimul oras vizitat la primul */
		crt_cost += d[path[n]][crt_city];
		/* afisare drum */
		printf("\nDrumul are costul %f format din:\n", crt_cost);
		for (i = 1; i <= n; i++) {
			printf("%d ", path[i]);
		}


		printf("%d\n\n", crt_city);
		if (crt_cost < best_cost) {
			for (i = 1; i <= n; i++)
				best_path[i] = path[i];
			best_cost = crt_cost;
		}
	}

	printf("\nCel mai scurt drum incepe din %d, are costul %f si este format din:\n", best_path[1], best_cost);
	for (i = 1; i <= n; i++)
		printf("%d ", best_path[i]);
	printf("%d\n\n\n\n", best_path[1]);

	return 0;
}

void choose_best(int last, double &min_dist, int &pos_min) {
	int j;
	/* Calculam distanta minima din punctul curent (last) catre un oras nevizitat pana acum */
	min_dist = MINIMUM; pos_min = -1;
	for (j = 1; j <= n; j++)
		if (!visited[j]) {
			if (d[last][j] < min_dist) {
				min_dist = d[last][j];
				pos_min = j;
			}
		}
}