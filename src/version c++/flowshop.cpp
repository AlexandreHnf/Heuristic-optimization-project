//
// Created by Alexandre on 0026 26 mars 2021.
//

/***************************************************************************
 *   Copyright (C) 2012 by Jérémie Dubois-Lacoste   *
 *   jeremie.dl@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <bits/stdc++.h>
#include "pfspinstance.h"
#include "flowshop.h"

using namespace std;

void printVector(vector<int> v, string message) {
    cout << message;
    for (int i=0; i<v.size(); i++){
        cout << v[i] << " ";
    }
    cout << endl;
}

void printSol(Solution s) {
    printVector(s.sol, "jobs : ");
    cout << "wct : " << s.wct << endl;
}

/***************************************************************************/

Solution generateRndSol(PfspInstance instance) {
    /*
    Fill the solution with numbers between 0 and nb_jobs, shuffled
    */
    std::vector<int> sol(instance.getNbJob());
    for (int i=0; i<instance.getNbJob(); i++) {
        sol[i] = i;
    }
    random_shuffle(sol.begin(), sol.end());
    Solution random_sol = {sol, instance.computeWCT(sol)};
    return random_sol;
}


vector<int> getInitSRZsol(PfspInstance instance) {
    /*
    Get the set of jobs ordered (ascending) according to their
    weighted sum of processing times
    */
    multimap<double, int > mymap; // sorted map in ascending order of the keys

    for (int i = 0; i < instance.getNbJob(); i++) {
        mymap.insert(make_pair(instance.weightedSumSingleJob(i), i));
    }

    vector<int> init_sol(0);
    // begin() returns to the first value of multimap.
    multimap<double,int> :: iterator it;
    for (it = mymap.begin() ; it != mymap.end() ; it++) {
        init_sol.push_back((*it).second);
//        cout << "(" << (*it).first << ", " << (*it).second << ")" << endl;
    }
    return init_sol;
}

Solution getBestSubset(int size, int job, vector<int> start, PfspInstance instance) {
    vector<int> subset = start;

    Solution best_subset = {vector<int>(0), static_cast<long>(10000000000)};
    for (int i = 0; i < size; i++){
        auto it = subset.begin();
        it = subset.insert ( it+i, job ); // insert job to position i
        int temp_wct = instance.computeWCT(subset);
//        printVector(subset, "subset : ");
//        cout << "=> " << temp_wct << endl;

        if (temp_wct <= best_subset.wct) {
            best_subset.sol = subset;
            best_subset.wct = temp_wct;
        }
        if (i < subset.size()) {
            subset.erase(subset.begin()+i); // remove the ith element of the list
        }
    }
    return best_subset;
}

Solution simplifiedRZheuristic(PfspInstance instance) {
    /*
    Start by ordering the jobs in ascending order of their weighted sum of processing times
	Then, construct the solution by inserting one job at a time in the position that minimize the WCT
    */
    vector<int> starting_sequence = getInitSRZsol(instance);
//    printVector(starting_sequence, "starting sequence : ");
    Solution best_sol = {vector<int>(0), static_cast<long>(10000000000)};
    for (int j = 0; j < instance.getNbJob(); j++) {
        Solution best_subset = getBestSubset(j+1, starting_sequence[j], best_sol.sol, instance);
//        printSol(best_subset);
        best_sol.sol = best_subset.sol;
        best_sol.wct = best_subset.wct;
    }
    return best_sol;

}

/***************************************************************************/

Solution getBestTransposeNeighbour(Solution sol, PfspInstance instance, string pivoting_rule) {
    /*
    Two permutations ø, ø' are transpose neighbours if, and only if,
	one can be obtained from the other by swapping the positions of two adjacent jobs
	ex : [A,B,C,D,E,F]
	=>   [A,C,B,D,E,F]
    */
    Solution best_sol = {vector<int>(0), sol.wct};
    vector<int> temp_sol;

    for (int i = 0; i < sol.sol.size()-1; i++) {
        temp_sol = sol.sol;
        iter_swap(temp_sol.begin() + i, temp_sol.begin() + i+1);
//        printVector(temp_sol, "");
        int wct = instance.computeWCT(temp_sol);
        if (wct < best_sol.wct) {
            best_sol.sol = temp_sol;
            best_sol.wct = wct;
            if (pivoting_rule == "FI") {
                return best_sol;
            }
        }
    }
    return best_sol;

}

Solution getBestExchangeNeighbour(Solution sol, PfspInstance instance, string pivoting_rule) {
    /*
    Two permutations ø, ø' are 2-exchange neighbours if, and only if,
    one can be obtained from the other by exchanging two jobs at arbitrary positions
    ex : [A,B,C,D,E,F]
    =>   [A,E,C,D,B,F]
    */
    Solution best_sol = {vector<int>(0), sol.wct};
    vector<int> temp_sol;

    for (int i = 0; i < sol.sol.size(); i++) {
        for (int j = 0; j < sol.sol.size(); j++) {
            if (i != j) {
                temp_sol = sol.sol;
                iter_swap(temp_sol.begin() + i, temp_sol.begin() + j);
//                printVector(temp_sol, "");
                int wct = instance.computeWCT(temp_sol);
                if (wct < best_sol.wct) {
                    best_sol.sol = temp_sol;
                    best_sol.wct = wct;
                    if (pivoting_rule == "FI") {
                        return best_sol;
                    }
                }
            }
        }
    }
    return best_sol;
}

Solution getBestInsertionNeighbour(Solution sol, PfspInstance instance, string pivoting_rule) {
    /*
    Two permutations ø, ø' are insertion neighbours if, and only if,
    one can be obtained from the other by removing a job from one position
                                                                  and inserting it at another position
    ex : [A,B,C,D,E,F]
                 =>   [A,C,D,B,E,F]

    QUESTION : l'insertion peuut se faire en fin de liste aussi ?
    */
    Solution best_sol = {vector<int>(0), sol.wct};
    vector<int> temp_sol;

    for (int i = 0; i < sol.sol.size(); i++) {
        for (int j = 0; j < sol.sol.size() + 1; j++) {
            if (i != j and j != i+1) {
                temp_sol = sol.sol;
                auto it = temp_sol.begin();
                it = temp_sol.insert ( it+j, sol.sol[i] ); // insert job to position j
                if (j < i) {
                    temp_sol.erase(temp_sol.begin()+i+1); // remove the (i+1)th element of the list
                } else {
                    temp_sol.erase(temp_sol.begin()+i); // remove the ith element of the list
                }
//                printVector(temp_sol, "");
                int wct = instance.computeWCT(temp_sol);
                if (wct < best_sol.wct) {
                    best_sol.sol = temp_sol;
                    best_sol.wct = wct;
                    if (pivoting_rule == "FI") {
                        return best_sol;
                    }
                }
            }
        }
    }
    return best_sol;
}

/***************************************************************************/


// ITERATIVE IMPROVEMENT
bool isLocalOptimal(Solution sol) {
    /*
    Check if the solution is a local optimum. By definition, a local optimum (here minimum) is :
    search position without improving neighbours w.r.t. given evaluation function g and neighbourhood
    N. i.e. position s in S such that g(s) <= g(s') for all s' of N(s).
    => same as checking if the neighbour found is empty. (if empty : sol is a local optimum)
    */
    return (sol.sol.empty()); // if true => local optimum
}

Solution generateInitialSolution(string mode, PfspInstance instance) {
    if (mode == "R") {
        return generateRndSol(instance);
    } else if (mode == "SRZ") {
        return simplifiedRZheuristic(instance);
    }
}

Solution chooseNeighbour(Solution sol, PfspInstance instance, string neighbour_type, string pivoting_rule) {
    /*
    Choose one neighbour (the best one) among all the neighbours of a given solution.
    */
    if (neighbour_type == "T") {
        return getBestTransposeNeighbour(sol, instance, pivoting_rule);
    } else if (neighbour_type == "E") {
        return getBestExchangeNeighbour(sol, instance, pivoting_rule);
    } else if (neighbour_type == "I"){
        return getBestInsertionNeighbour(sol, instance, pivoting_rule);
    }
}

Solution iterativeImprovement(string pivoting_rule, string neigh_rule, PfspInstance instance, Solution init_sol) {
    /*
    Heuristic algorithm to find the optimal solution to the PSFP
    args = [pivotingrule, neighbourhood]
    */
    Solution best_sol = init_sol;
    Solution neighbour = chooseNeighbour(best_sol, instance, neigh_rule, pivoting_rule);
    int it = 0;
    while (!isLocalOptimal(neighbour)) {
        best_sol = neighbour;
        neighbour = chooseNeighbour(best_sol, instance, neigh_rule, pivoting_rule);
        it++;
    }
    return best_sol;
}

// VND : variable neighbourhood descent
Solution variableNeighbourhoodDescent(vector<string> neighbourhood_modes, PfspInstance instance, Solution init_sol) {
    /*
    VND variable neighbourhood descent
    use different neighbourhood mode throughout the algorithm
    args = [name.py, initsol, pivotingrule, neighbourhood]
    */
    int i = 0;
    int it = 0;
    bool stop = false;
    Solution best_sol = init_sol;
    while (not stop) { // "do while"
//        cout << "neighbourhood mode : " << neighbourhood_modes[i] << endl;
        Solution neighbour = chooseNeighbour(best_sol, instance, neighbourhood_modes[i], "FI");
        if (isLocalOptimal(neighbour)) { // if no existing improving solution
            i++;
        } else {
            best_sol = neighbour;
//            cout << "it " << it << " | ";
//            printSol(best_sol);
            i = 0;
            it++;
        }
        stop = i > neighbourhood_modes.size() - 1;
    }
    return best_sol;
}

vector<vector<int>> readBestSolFromFile(char * filename) {
    // TODO : must be working on ubuntu and windows with relative path

    bool everythingOK = true;
    double wct;
    string str;
    ifstream fileIn;

    vector<vector<int>> best_solutions = {vector<int>(), vector<int>()};

    fileIn.open(filename);

    if ( fileIn.is_open() ) {
        fileIn >> str; fileIn >> str; fileIn >> str; // "Problem , BS" => skip first line
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 30; ++j) {
                fileIn >> str; // The instance name, not important !
                fileIn >> str; // "," not important
                fileIn >> wct; // best solution wct
                cout << wct << endl;
                best_solutions[i].push_back( (int) wct);
            }
        }

        cout << "All is read from file." << std::endl;
        fileIn.close();
    }
    else {
        cout    << "ERROR. file:pfspInstance.cpp, method:readBestSolFromFile, "
                << "error while opening file " << filename << endl;
        everythingOK = false;
    }

    return best_solutions;
}