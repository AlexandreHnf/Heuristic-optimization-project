//
// Created by Alexandre on 0026 26 mars 2021.
//

#ifndef VERSION_C___FLOWSHOP_H
#define VERSION_C___FLOWSHOP_H

#include <string>
#include <vector>

struct Solution
{
    vector<int> sol;
    long int wct;
};
void printVector(vector<int>, string);
void printSol(Solution s);

// INITIAL SOLUTION GENERATION
Solution generateRndSol(PfspInstance instance);
vector<int> getInitSRZsol(PfspInstance instance);
Solution getBestSubset(int size, int job, vector<int> start, PfspInstance instance);
Solution simplifiedRZheuristic(PfspInstance instance);

// NEIGHBOUR SOLUTIONS GENERATION
Solution getBestTransposeNeighbour(Solution sol, PfspInstance instance, string pivoting_rule);
Solution getBestExchangeNeighbour(Solution sol, PfspInstance instance, string pivoting_rule);
Solution getBestInsertionNeighbour(Solution sol, PfspInstance instance, string pivoting_rule);

// ITERATIVE IMPROVEMENT
bool isLocalOptimal(Solution sol);
Solution generateInitialSolution(string mode, PfspInstance instance);
Solution chooseNeighbour(Solution sol, PfspInstance instance, string neighbour_type, string pivoting_rule);
Solution iterativeImprovement(string pivoting_rule, string neigh_rule, PfspInstance instance, Solution init_sol);

// VND : variable neighbourhood descent
Solution variableNeighbourhoodDescent(vector<string> neighbourhood_modes, PfspInstance instance, Solution init_sol);

// For tests
//double averageRelativePercentageDev()

vector<vector<int>> readBestSolFromFile(char * filename);

#endif //VERSION_C___FLOWSHOP_H
