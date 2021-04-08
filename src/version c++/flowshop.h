//
// Created by Alexandre HENEFFE on 0026 26 mars 2021.
//

#ifndef VERSION_C___FLOWSHOP_H
#define VERSION_C___FLOWSHOP_H

#include <string>
#include <vector>

struct Solution
{
    std::vector<int> sol;
    long int wct;
};
void printVector(std::vector<int>, std::string);
void printSol(Solution s);

// INITIAL SOLUTION GENERATION
Solution generateRndSol(PfspInstance instance);
std::vector<int> getInitSRZsol(PfspInstance instance);
Solution getBestSubset(int size, int job, std::vector<int> start, PfspInstance instance);
Solution simplifiedRZheuristic(PfspInstance instance);

// NEIGHBOUR SOLUTIONS GENERATION
Solution getBestTransposeNeighbour(Solution sol, PfspInstance instance, std::string pivoting_rule);
Solution getBestExchangeNeighbour(Solution sol, PfspInstance instance, std::string pivoting_rule);
Solution getBestInsertionNeighbour(Solution sol, PfspInstance instance, std::string pivoting_rule);

// ITERATIVE IMPROVEMENT
bool isLocalOptimal(Solution sol);
Solution generateInitialSolution(std::string mode, PfspInstance instance);
Solution chooseNeighbour(Solution sol, PfspInstance instance, std::string neighbour_type, std::string pivoting_rule);
Solution iterativeImprovement(std::string pivoting_rule, std::string neigh_rule, PfspInstance instance, Solution init_sol);

// VND : variable neighbourhood descent
Solution variableNeighbourhoodDescent(std::vector<std::string> neighbourhood_modes, PfspInstance instance, Solution init_sol);

// For tests
//double averageRelativePercentageDev()

std::vector<std::vector<int>> readBestSolFromFile(std::basic_string<char> filename);

#endif //VERSION_C___FLOWSHOP_H
