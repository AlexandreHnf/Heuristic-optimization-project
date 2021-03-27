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
    cout << "-----" << endl;
    printVector(s.sol, "jobs : ");
    cout << "wct : " << s.wct << endl;
}

std::vector<int> generateRndSol(int nbJ) {
    std::vector<int> sol(nbJ);
    for (int i=0; i<nbJ; i++) {
        sol[i] = i;
    }
    random_shuffle(sol.begin(), sol.end());
    return sol;
}

/***********************************************************************/

vector<int> getInitSRZsol(PfspInstance instance) {
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