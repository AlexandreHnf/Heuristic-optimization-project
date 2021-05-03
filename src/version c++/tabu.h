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


void insertRight(int a, int b, vInt &temp_sol) {
    int to_insert = temp_sol[a];
    auto it = temp_sol.begin();
    it = temp_sol.insert ( it+b+1, to_insert ); // insert job to position j
    temp_sol.erase(temp_sol.begin()+a); // remove the (i+1)th element of the list
}

void insertLeft(int a, int b, vInt &temp_sol) {
    int to_insert = temp_sol[a];
    auto it = temp_sol.begin();
    it = temp_sol.insert ( it+b, to_insert ); // insert job to position j
    temp_sol.erase(temp_sol.begin()+a+1); // remove the (i+1)th element of the list
}

bool isTabu(int a, int b, vInt currsol, vvInt tabu_list, vInt move) {
    if (a < b) {
        for (int k=a+1; k <= b; k++) {
            for (auto pair: tabu_list) {
                // (pi(j), pi(a)), j = a+1 ,..., b
                if (pair[0] == currsol[k] and pair[1] == currsol[a]) {
                    return true;
                }
            }
        }

    } else if (a > b) {
        for (int k=b; k <= a-1; k++) {
            for (auto pair: tabu_list) {
                // (pi(a), pi(j)), j = b ,..., a-1
                if (pair[0] == currsol[a] and pair[1] == currsol[k]) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool aspirationCriterion(int a, int b, vInt candidate_sol, vvInt tabu_list, vInt move, int wct, int best_wct) {
    bool is_tabu = isTabu(a, b, candidate_sol, tabu_list, move);

    if (! is_tabu) {
        return true;
    }
    // aspiration criterion : if tabu but better than best, then still taken
    if (is_tabu and wct < best_wct) {
        return true;
    }
    return false;  // if tabu
}

Solution getBestNeighbour(PfspInstance instance, Solution candidate, vvInt &tabu_list, int best_wct) {
    Solution best_neigh = {vInt(0), candidate.wct};
    vInt temp_sol, move, best_move;

    // test all the pairs of positions
    for (int a = 0; a < candidate.sol.size(); a++) {
        for (int b = 0; b < candidate.sol.size(); b++) {
            if (a != b ) { // and b != a+1
                temp_sol = candidate.sol;
                if (a < b) {
                    insertRight(a, b, temp_sol);
                    move = {candidate.sol[a], candidate.sol[a+1]};  // (pi(a), pi(a+1))
                } else if (a > b) {
                    insertLeft(a, b, temp_sol);
                    move = {candidate.sol[a-1], candidate.sol[a]};  // (pi(a-1), pi(a))
                }

                int wct = instance.computeWCT(temp_sol);
                // aspiration criterion : if tabu but better than best, then still taken
                if (aspirationCriterion(a, b, candidate.sol, tabu_list, move, wct, best_wct)) {
                    if (wct < best_neigh.wct) {
                        best_neigh.sol = temp_sol;
                        best_neigh.wct = wct;
                        best_move = move;
                    }
                }
            }
        }
    }
    tabu_list.push_back(best_move); // add new best move to tabu list
    return best_neigh;
}

bool terminationCriterion() {
    return true;
}

void printTL(vvInt tabu_list) {
    cout << "TABUs: ";
    for (auto pair: tabu_list) {
        cout << "(" << pair[0] << "," << pair[1] << "), ";
    }
    cout << endl;
}

Solution tabuSearch(PfspInstance instance, int max_tabu_size) {
    Solution init_sol = generateRndSol(instance);
    Solution best_sol = init_sol;
    vvInt tabu_list;
    Solution best_candidate = init_sol;
    int it = 0;
//    while (! terminationCriterion()) {
    while (it < 100) {
        cout << "it : " << it << ", " << best_sol.wct << endl;
        best_candidate = getBestNeighbour(instance, best_candidate, tabu_list, best_sol.wct);
        printSol(best_candidate);
        if (best_candidate.sol.size() == 0)  // if no improving neighbour
            return best_sol;

        if (best_candidate.wct < best_sol.wct) {
            best_sol = best_candidate;
        }
        // update tabu list
        if (tabu_list.size() > max_tabu_size) {
            tabu_list.erase(tabu_list.begin()+0); // pop front
        }
        it++;
        printTL(tabu_list);
    }
    return best_sol;

}

#endif //VERSION_C___TABU_H
