//
// Created by Alexandre on 0026 26 mars 2021.
//

#ifndef VERSION_C___FLOWSHOP_H
#define VERSION_C___FLOWSHOP_H

#include <string>
#include <vector>

int generateRndPosition(int min, int max);
void randomPermutation(int nbJobs, std::vector< int > & sol);
std::vector<int> generateRndSol(int nbJobs);

#endif //VERSION_C___FLOWSHOP_H
