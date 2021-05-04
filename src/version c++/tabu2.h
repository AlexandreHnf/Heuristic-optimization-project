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

void updateTabuList(vvInt &tabu_list, int max_tabu_size, vInt best_move) {
    // update tabu list
    if (best_move.size() == 0)
        best_move = {-1, -1};  // add not significant move (to preserve order)

    tabu_list.push_back(best_move); // add new best move to tabu list
    if (tabu_list.size() > max_tabu_size) {
        tabu_list.erase(tabu_list.begin()+0); // pop front
    }
}

void evaluateNeighbour(int wct, vSol &best_neighs, vvInt &best_moves, vInt move, vInt temp_sol) {
    if (wct < best_neighs[0].wct) {
        best_neighs[0].sol = temp_sol;
        best_neighs[0].wct = wct;
        best_moves[0] = move;
    }
    // if no improving neighbour, then we take worsening step to
    // escape from local optimum
    if (wct > best_neighs[1].wct and getRandom01() < 0.2) {
        best_neighs[1].sol = temp_sol;
        best_neighs[1].wct = wct;
        best_moves[1] = move;
    }
}

vInt getMove(vInt candidate_sol, int a, int b, vInt &temp_sol) {
    if (a < b) {
        insertRight(a, b, temp_sol);
        return {candidate_sol[a], candidate_sol[a+1]};  // (pi(a), pi(a+1))
    } else if (a > b) {
        insertLeft(a, b, temp_sol);
        return {candidate_sol[a-1], candidate_sol[a]};  // (pi(a-1), pi(a))
    } else {
        return {-1, -1};
    }
}

int chooseBestNeighbour(vSol best_neighs, vvInt best_moves) {
    if (best_neighs[0].sol.size() == 0) {
        return 1;
    } else {
        return 0;
    }
}

Solution getBestNeighbour(PfspInstance instance, Solution candidate, vvInt &tabu_list, int best_wct, int max_tabu_size) {
//    vSol best_neighs = {{vInt(0), candidate.wct}, {vInt(0), std::numeric_limits<int>::max()}};
    vSol best_neighs = {{vInt(0), candidate.wct}, {vInt(0), 0}};
    vvInt best_moves(2);
    vInt temp_sol, move;
    int nb_accepted = 0;
    int tot = 0;

    // test all the pairs of positions
    for (int a = 0; a < candidate.sol.size(); a++) {
        for (int b = 0; b < candidate.sol.size(); b++) {
            if (a != b ) { // and b != a+1
                temp_sol = candidate.sol;
                move = getMove(candidate.sol, a, b, temp_sol);

                int wct = instance.computeWCT(temp_sol);
                // aspiration criterion : if tabu but better than best, then still taken
                if (aspirationCriterion(a, b, candidate.sol, tabu_list, move, wct, best_wct)) {
                    evaluateNeighbour(wct, best_neighs, best_moves, move, temp_sol);
                    nb_accepted++;
                }
                tot++;
            }
        }
    }
//    cout << nb_accepted << "/" << tot << " accepted" << endl;
    int chosen_neigh = chooseBestNeighbour(best_neighs, best_moves);
    updateTabuList(tabu_list, max_tabu_size, best_moves[chosen_neigh]);
    return best_neighs[chosen_neigh];
}

bool terminationCriterion(int it) {
    return it >= 1000;
}

void printTL(vvInt tabu_list) {
    cout << "TABUs: ";
    for (auto pair: tabu_list) {
        cout << "(" << pair[0] << "," << pair[1] << "), ";
    }
    cout << endl;
}

Solution tabuSearch(PfspInstance instance, int max_tabu_size) {
    cout << "tabu without opti" << endl;
    Solution init_sol = generateRndSol(instance);
//    Solution init_sol = simplifiedRZheuristic(instance);
    Solution best_sol = init_sol;
    vvInt tabu_list;
    Solution best_candidate = init_sol;
    int it = 0;
    while (! terminationCriterion(it)) {
        cout << "it " << it << ", B : " << best_sol.wct;
        it++;
        best_candidate = getBestNeighbour(instance, best_candidate, tabu_list, best_sol.wct, max_tabu_size);
//        printTL(tabu_list);
        if (best_candidate.sol.size() == 0) // if no improving neighbour
            best_candidate = best_sol;
        cout << " C : " << best_candidate.wct << endl;

        if (best_candidate.wct < best_sol.wct) {
            best_sol = best_candidate;
        }
    }
    return best_sol;

}

#endif //VERSION_C___TABU_H
