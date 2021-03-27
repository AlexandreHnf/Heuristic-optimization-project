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

std::vector<int> generateRndSol(int nbJobs);

vector<int> getInitSRZsol(PfspInstance instance);
Solution getBestSubset(int size, int job, vector<int> start, PfspInstance instance);
Solution simplifiedRZheuristic(PfspInstance instance);

#endif //VERSION_C___FLOWSHOP_H
