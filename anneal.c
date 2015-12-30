/**
 * Simulated annealing algorithm for the D-Wave problem
 * Copyright (C) 2014 Anirudha Bose
 * Code released under the The MIT License (MIT)
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* Simulated annealed algorithm parameters */
#define INITIAL_TEMPERATURE	0.0012
#define COOLING_STEPS		100000
#define COOLING_FACTOR		0.999997
#define HIDDEN_ENERGY		512  /* Hidden potential energy to increase success probability */
#define POTENTIAL_ENERGY	1024 /* Add some potential energy to the Hamiltonian to keep it positive */

/* D-Wave parameters */
#define QUBITS_NUM		108  /* Number of qubits used in D-Wave Benchmarks */
#define COUPLINGS		255  /* Number of couplings in each instance */

int Z[QUBITS_NUM];
int nodes[COUPLINGS][3];
int actualHamiltonian;
int optimalHamiltonian;

void dataBinaryCouplings(void)
{
    int i,j,k;
    scanf("%d\n", &actualHamiltonian);
    for (k=0; k<COUPLINGS; k++)
    {
        scanf("%d %d %d\n",&i,&j,&nodes[k][2]);
        nodes[k][0] = i-1;
        nodes[k][1] = j-1;
    }
}

int getHamiltonian(void)
{
    int k, H=POTENTIAL_ENERGY;
    for(k=0; k<COUPLINGS; k++)
        H -= nodes[k][2] * Z[nodes[k][0]] * Z[nodes[k][1]];
    return H;
}

void initQubits(void)
{
    int i;
    /*For details about the pseudo-random number generator used, see http://stackoverflow.com/a/7343909 */
    srand(time(NULL));
    struct timeval t1;
    gettimeofday(&t1, NULL);
    srand(t1.tv_usec * t1.tv_sec);
    for (i=0; i<QUBITS_NUM; i++)
        Z[i] = rand()%2 ? 1 : -1;
}

int anneal(void)
{
    int i;
    double threshold;
    double T;
    int currentHamiltonian;
    int dH;
    int randomQubit = 0;

    T = INITIAL_TEMPERATURE;
    currentHamiltonian   = HIDDEN_ENERGY + POTENTIAL_ENERGY;
    optimalHamiltonian   = currentHamiltonian;

    for (i=0; i<COOLING_STEPS; i++)
    {
        randomQubit = rand()%QUBITS_NUM;
        Z[randomQubit] = -1 * Z[randomQubit];
        dH = getHamiltonian() - currentHamiltonian;
        if(dH < 0)
        {
            currentHamiltonian = currentHamiltonian + dH;
            T *= COOLING_FACTOR;
        }
        else
        {
            threshold = expf(-(float)dH/(currentHamiltonian * T));
            if (threshold > rand()/(float)RAND_MAX)
            {
                currentHamiltonian = currentHamiltonian + dH;
                if (dH == 0)
                    T *= (1-(1-COOLING_FACTOR)/7.0);
            }
            else
                Z[randomQubit] = -1 * Z[randomQubit]; /* Go back to initial state of Z */
        }

        if (currentHamiltonian < optimalHamiltonian)
            optimalHamiltonian = currentHamiltonian;
    }
    return optimalHamiltonian;
}

int main(void)
{
    dataBinaryCouplings();
    initQubits();
    int predictedHamiltonian = anneal();
    (actualHamiltonian == predictedHamiltonian - POTENTIAL_ENERGY) ? printf("SUCCESS\n") : printf("FAILURE\n");
    return 0;
}