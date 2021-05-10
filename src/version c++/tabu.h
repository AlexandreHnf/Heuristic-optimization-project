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
#include "Utils.h"

using namespace std;
typedef vector<int> vInt;
typedef vector<vector<int>> vvInt;
typedef vector<Solution> vSol;

float getRand01() {
    return ((double) rand() / (RAND_MAX));
}

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

bool isTabu(int a, int b, vInt currsol, vvInt tabu_mat) {
    if (a < b) {
        for (int k=a+1; k <= b; k++) {
            // (pi(j), pi(a)), j = a+1 ,..., b
            if (tabu_mat[currsol[k]][currsol[a]] > 0) {
                return true;
            }
        }

    } else if (a > b) {
        for (int k=b; k <= a-1; k++) {
            // (pi(a), pi(j)), j = b ,..., a-1
            if (tabu_mat[currsol[a]][currsol[k]] > 0) {
                return true;
            }
        }
    }
    return false;
}

bool aspirationCriterion(int a, int b, vInt candidate_sol, vvInt tabu_mat, int wct, int best_wct) {
    bool is_tabu = isTabu(a, b, candidate_sol, tabu_mat);

    if (! is_tabu) {
        return true;
    }
    // aspiration criterion : if tabu but better than best, then still taken
    if (is_tabu and wct < best_wct) {
        return true;
    }
    return false;  // if tabu
}

void updateTabuList(vvInt &tabu_mat, int max_tabu_size, vInt best_move) {
    // update tabu list

    if (best_move[0] != -1 and best_move[1] != -1)
        tabu_mat[best_move[0]][best_move[1]] = max_tabu_size;

    // decrease tabu status
    for (int i = 0; i < tabu_mat.size(); i++) {
        for (int j = 0; j < tabu_mat.size(); j++) {
            if (tabu_mat[i][j] > 0) {
                tabu_mat[i][j]--;
            }
        }
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
    if (wct > best_neighs[1].wct and getRand01() < 0.2) {
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

Solution getBestNeighbour(PfspInstance instance, Solution candidate, vvInt &tabu_mat, int best_wct, int max_tabu_size) {
//    vSol best_neighs = {{vInt(0), candidate.wct}, {vInt(0), std::numeric_limits<int>::max()}};
    vSol best_neighs = {{vInt(0), candidate.wct}, {vInt(0), 0}};
    vvInt best_moves(2);
    vInt temp_sol, move;
    int nb_accepted = 0;
    int tot = 0;

    // test all the pairs of positions
    for (int a = 0; a < candidate.sol.size(); a++) {
        for (int b = 0; b < candidate.sol.size(); b++) {
            if (a != b ) {
                temp_sol = candidate.sol;
                move = getMove(candidate.sol, a, b, temp_sol);

                int wct = instance.computeWCT(temp_sol);
                // aspiration criterion : if tabu but better than best, then still taken
                if (aspirationCriterion(a, b, candidate.sol, tabu_mat, wct, best_wct)) {
                    evaluateNeighbour(wct, best_neighs, best_moves, move, temp_sol);
                    nb_accepted++;
                }
                tot++;
            }
        }
    }
//    cout << nb_accepted << "/" << tot << " accepted" << endl;
    int chosen_neigh = chooseBestNeighbour(best_neighs, best_moves);
    updateTabuList(tabu_mat, max_tabu_size, best_moves[chosen_neigh]);
    return best_neighs[chosen_neigh];
}


bool terminationCriterion(int it, chrono::steady_clock::time_point start_time, double max_time) {
    auto now = chrono::steady_clock::now();
    double timing = chrono::duration <double> (now-start_time).count();
    if (timing >= max_time)
        return true;

    return false;
}

vvInt initTabuMatrix(PfspInstance instance) {
    vvInt tabu_matrix;
    for (int i = 0; i < instance.getNbJob(); i++) {
        vInt lign(instance.getNbJob(), 0);
        tabu_matrix.push_back(lign);
    }
    return tabu_matrix;
}

Solution tabuSearch(PfspInstance instance, int max_tabu_size, timing start_time, double max_time, vdouble &best_runtimes) {
    Solution init_sol = generateRndSol(instance);
    Solution best_sol = init_sol;
    vvInt tabu_mat = initTabuMatrix(instance);
    Solution best_candidate = init_sol;
    int it = 0;
    while (! terminationCriterion(it, start_time, max_time)) {
        it++;
        best_candidate = getBestNeighbour(instance, best_candidate, tabu_mat, best_sol.wct, max_tabu_size);
        if (best_candidate.sol.empty()) // if no improving neighbour
            best_candidate = best_sol;
//        cout << "it " << it << ", B : " << best_sol.wct << " C : " << best_candidate.wct << endl;

        if (best_candidate.wct < best_sol.wct) {
            best_sol = best_candidate;
        }
        if (!best_runtimes.empty())
            updateBestRuntimes(instance, best_runtimes, best_sol, start_time, max_time);
    }
    return best_sol;
}

#endif //VERSION_C___TABU_H
