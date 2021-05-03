//
// Created by Alexandre on 0029 29 avril 2021.
//

#ifndef VERSION_C___TABU_H
#define VERSION_C___TABU_H

#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <cstdlib>     /* srand, rand */
#include <bits/stdc++.h>
#include "pfspinstance.h"
#include "flowshop.h"

using namespace std;
typedef vector<int> vInt;
typedef vector<vector<int>> vvInt;
typedef vector<Solution> vSol;

bool aspirationCriterion(Solution best_sol, Solution candidate_sol) {
    return true;
}

bool isTabu(vInt temp_sol, vSol tabu_list) {
    for (auto s: tabu_list) {
        if (s.sol == temp_sol) {
            return true;
        }
    }
    return false;
}

Solution getBestNeighbour(PfspInstance instance, Solution candidate, vSol tabu_list) {
    Solution best_sol = {vInt(0), candidate.wct};
    vInt temp_sol;

    for (int i = 0; i < candidate.sol.size(); i++) {
        for (int j = 0; j < candidate.sol.size() + 1; j++) {
            if (i != j and j != i+1) {
                temp_sol = candidate.sol;
                auto it = temp_sol.begin();
                it = temp_sol.insert ( it+j, candidate.sol[i] ); // insert job to position j
                if (j < i) {
                    temp_sol.erase(temp_sol.begin()+i+1); // remove the (i+1)th element of the list
                } else {
                    temp_sol.erase(temp_sol.begin()+i); // remove the ith element of the list
                }
                int wct = instance.computeWCT(temp_sol);
                if (wct < best_sol.wct and !isTabu(temp_sol, tabu_list)) {
                    best_sol.sol = temp_sol;
                    best_sol.wct = wct;
                }
            }
        }
    }
    return best_sol;
}

Solution tabuSearch(PfspInstance instance, int max_tabu_size) {
    Solution init_sol = generateRndSol(instance);
    Solution best_sol = init_sol;
    vSol tabu_list = {init_sol};
    Solution best_candidate = init_sol;
    while (! aspirationCriterion(best_sol, best_candidate)) {
        best_candidate = getBestNeighbour(instance, best_candidate, tabu_list);

        if (best_candidate.wct < best_sol.wct) {
            best_sol = best_candidate;
        }
        tabu_list.push_back(best_candidate);
        if (tabu_list.size() > max_tabu_size) {
            tabu_list.erase(tabu_list.begin()+0); // pop front
        }
    }
    return best_sol;

}

#endif //VERSION_C___TABU_H
