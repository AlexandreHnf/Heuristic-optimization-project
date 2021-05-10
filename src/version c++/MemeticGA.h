//
// Created by Alexandre on 0026 26 avril 2021.
//

#ifndef VERSION_C___MEMETICGA_H
#define VERSION_C___MEMETICGA_H


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
typedef vector<Solution> Population;


int getRandomIndex(int start, int end, int to_avoid) {
    /* generate secret number between 0 and n different from to_avoid */
    int index = rand() % end + start;
    while (index == to_avoid) {
        index = rand() % end + start;
    }
    return index;
}

float getRandom01() {
    return ((double) rand() / (RAND_MAX));
}

void printPop(Population pop) {
    for (auto p: pop) {
        printVector(p.sol, "");
    }
}

Population generateFullRNDpop(PfspInstance instance, int N) {
    /*
     * Generate a fully random population
     */
    vector<Solution> pop;
    for (int i = 0; i < N; i++) {
        pop.push_back(generateRndSol(instance));
    }
    return pop;
}

Population generateInitPopulation(PfspInstance instance, int N) {
    /*
     * Seeding mechanism : assuming a population of M chromosomes, the M-1
     * chromosomes are generated at random while a single chromosome is generated
     * using SRZ heuristic (Nawaz et al, 1983.)
     */
    vector<Solution> pop;
    Solution first_chrom = simplifiedRZheuristic(instance);
    pop.push_back(first_chrom);
    for (int i = 0; i < N-1; i++) {
        pop.push_back(generateRndSol(instance));
    }
    return pop;
}

void evaluatePopulation(PfspInstance instance, Population pop) {
    for (auto c : pop) {
        c.wct = instance.computeWCT(c.sol);
    }
}

Solution getBestChromosome(Population pop) {
    Solution best_sol = pop[0];
    for (int i = 1; i < pop.size(); i++) {
        if (pop[i].wct < best_sol.wct) {
            best_sol = pop[i];
        }
    }
    return best_sol;
}

Population getSortedPopulation(Population pop) {
    /*
     * Sort the chromosomes in ascending order according to their
     * fitness
     */
    multimap<double, int> mymap;  // sorted map in ascending order of the keys
    for (int i = 0; i < pop.size(); i++) {
        mymap.insert(make_pair(- pop[i].wct, i));  // -wct so that lower wct = better
    }
    Population sorted(0);
    multimap<double,int>::iterator it;
    for (it = mymap.begin(); it != mymap.end(); it++){
        sorted.push_back(pop[(*it).second]);
    }
    return sorted;
}

Population eliteSelection(float Pe, Population pop) {
    /*
     * get Pe% of best chromosomes (last Pe% of the sorted pop)
     * ex : [1,2,3,4,5,6,7,8,9,10] , Pe = 60% => [5,6,7,8,9,10]
     */
    Population sorted = getSortedPopulation(pop);
    int N = pop.size();
    int offset = N - Pe*N;
    Population elite_pop = std::vector<Solution>(sorted.begin() + offset, sorted.end());
    return elite_pop;
}

vInt findMaxValue(vvint M) {
    /*
     * find the element Mij with the maximum unvisited value in M
     */
    vInt max_cell = {0,0};
    int max_val = 0;
    for (int i = 0; i < M.size(); i++) {
        for (int j = 0; j < M[0].size(); j++) {
            if (M[i][j] > max_val) {
                max_cell = {i, j};
                max_val = M[i][j];
            }
        }
    }
    return max_cell;
}

Solution getArtificialChromosome(PfspInstance instance, vvInt M, int n) {
    /*
     * From the dominance matrix M containing occurences of jobs
     * at different positions, returns the artificial chromosome
     */
    vInt artificial_chromosome(n);
    int count = 0;
    while (count < n) {
        // find the element Mij with the maximum unvisited value in M
        vInt max_cell = findMaxValue(M);
        artificial_chromosome[max_cell[1]] = max_cell[0];
        // set all elements in row i and column j as -1
        for (int j = 0; j < M[0].size(); j++)  // col
            M[max_cell[0]][j] = -1;
        for (int i = 0; i < M.size(); i++)  // col
            M[i][max_cell[1]] = -1;
        count++;
    }
    Solution ac = {artificial_chromosome, instance.computeWCT(artificial_chromosome)};
    return ac;
}

void showDominanceMatrix(vvInt M) {
    for (int i = 0; i < M.size(); i++) {
        for (int j = 0; j < M[0].size(); j++) {
            cout << M[i][j] << " ";
        }
        cout << endl;
    }
}

Solution WSMGS(PfspInstance instance, Population E) {
    /*
     * Weighted simple mining gene structure
     * Voting : given some chromosomes, construct a dominance matrix
     * keeping the nb of occurences of each job at each position
     * Then, get the artificial chromosome out of this matrix
     */

    // Voting
    int value;
    vvInt M(instance.getNbJob(), vInt(instance.getNbJob()));
    for (int i = 0; i < instance.getNbJob(); i++) {  // jobs
        for (int j = 0; j < instance.getNbJob(); j++) {  // positions
            value = 0;
            for (int k = 0; k < E.size(); k++) {
                if (E[k].sol[j] == i) {  // if equal to job i at position j in sol k /
                    value += k+1;  // k = P = position of the chromosome in E + 1
                }
            }
            M[i][j] = value;
        }
    }
//    printPop(E);
//    showDominanceMatrix(M);
    Solution artificial_chromosome = getArtificialChromosome(instance, M, instance.getNbJob());
//    printSol(artificial_chromosome);
    return artificial_chromosome;
}

int rouletteWheelSelection(Population pop) {
    /*
     * Ranking selection method and then :
     * Roulette Wheel selection
     * rank = 1 = worst, rank = N = best.
     * pop is sorted in ascending order of solution quality = lowest wct = highest rank
     */
    int sum_ranks = pop.size()*(pop.size()+1)/2;  // prob = rank/N
    int tot_sum = 0;
    for (int i=0; i < pop.size(); i++) {
        tot_sum += i+1/sum_ranks;
    }
    int rand = getRandomIndex(0, tot_sum, -1);
    int partial_sum = 0;
    Solution selected;
    for (int x = 0; x < pop.size(); x++) {
        partial_sum += x+1;
        if (rand <= partial_sum) {
            return x;
        }
    }
    return -1;
}

vInt getCommonJobs(Solution c1, Solution c2) {
    /*
     * For a set of chromosomes, a job is called a Common Job
     * if it locates at the identical slot (pos) in all the chromosomes
     */
    vInt common_jobs(0);  // indices of the common jobs of c1 and c2
    for (int i = 0; i < c1.sol.size(); i++) {
        if (c1.sol[i] == c2.sol[i]) {
            common_jobs.push_back(i);
        }
    }
    return common_jobs;
}

Solution bestParent(Solution p1, Solution p2) {
    if (p1.wct < p2.wct) {
        return p1;
    } else {
        return p2;
    }
}

void leftCrossover(PfspInstance instance, Solution p1, Solution p2, vInt &child, int crossover_point) {
    // the child inherits all the unassigned jobs before the crossover points from parent 1
    for (int i = 0; i < crossover_point; i++) {
        if (child[i] == -1) {
            child[i] = p1.sol[i];
        }
    }
}

void rightCrossover(PfspInstance instance, Solution p1, Solution p2, vInt &child) {
    // The other elements of the child after the crossover point are copied in the same
    // order from parent2's chromosome
    for (int i = 0; i < instance.getNbJob(); i++) {
        bool allowed = true;
        int index = -1;
        for (int j = 0; j < instance.getNbJob(); j++) {
            if (p2.sol[i] == child[j]) {
                allowed = false;
                break;
            } else {
                if (child[j] == -1 and index == -1) {
                    index = j;
                }
            }
        }
        if (allowed)
            child[index] = p2.sol[i];
    }
}

Solution crossover(PfspInstance instance, Solution p1, Solution p2, Solution artificial_chromosome, float Pc) {
    /*
     * Crossover between parent 1 and parent 2 based on artificial chromosome
     * returns a child with good genes from all parents
     * p1, p2 are either Father,Mother or Mother,Father
     * TODO : consider Pc
     */
    Solution new_child = bestParent(p1, p2);

    if (getRandom01() < Pc) {
        int crossover_point = getRandomIndex(1, instance.getNbJob(), -1);

        // search for common jobs
        vInt CJp1p2 = getCommonJobs(p1, p2);
        vInt CJp1A = getCommonJobs(p1, artificial_chromosome);

        // transfer Common Jobs to child with positions unchanged
        vInt child(instance.getNbJob(), -1);  // -1 = empty cell
        for (int i : CJp1p2)
            child[i] = p1.sol[i];
        for (int j : CJp1A)
            child[j] = p1.sol[j];

        leftCrossover(instance, p1, p2, child, crossover_point);
        rightCrossover(instance, p1, p2, child);

        new_child = {child, instance.computeWCT(child)};
    }
    return new_child;

}

void mutation(Solution &child, float Pm) {
    /*
    Shift mutation : A single job is selected randomly and inserted in a
    random position
    */
    if (getRandom01() < Pm) {
        int elem_index = getRandomIndex(0, child.sol.size(), -1);  // rand index
        int new_pos = getRandomIndex(0, child.sol.size(), elem_index);  // != from elem_index
//        cout << "elem index : " << elem_index << ", new pos : " << new_pos << endl;

        auto it = child.sol.begin();
        it = child.sol.insert(it + new_pos, child.sol[elem_index]);
        if (new_pos < elem_index) {
            child.sol.erase(child.sol.begin()+elem_index+1); // remove the (i+1)th element of the list
        } else {
            child.sol.erase(child.sol.begin()+elem_index); // remove the ith element of the list
        }
    }
}

void reproduction(PfspInstance instance, int N, float Pc, float Pm, Population pop, Population &offspring_pop, Solution artificial_chromosome) {

    // repeat until covering the entire population :
    vInt covered(N, false);
    while (offspring_pop.size() < pop.size()) {
        int f, m;
        // father and mother must be different
        do {f = rouletteWheelSelection(pop); m = rouletteWheelSelection(pop);} while(f==m);
        Solution father = pop[f]; covered[f] = true;
        Solution mother = pop[m]; covered[m] = true;
        Solution son = crossover(instance, father, mother, artificial_chromosome, Pc);
        Solution daughter = crossover(instance, mother, father, artificial_chromosome, Pc);

        mutation(son, Pm);
        mutation(daughter, Pm);

        // local search to improve both son and daughter
        // Random - First improvement - Insert seems to be the best Iterative improvement algo
        son = iterativeImprovement("FI", "I", instance, son);
        daughter = iterativeImprovement("FI", "I", instance, daughter);

        offspring_pop.push_back(son);
        offspring_pop.push_back(daughter);
    }
}

bool terminationCriterion(timing start_time, double max_time, int count, int COUNT) {
    auto now = chrono::steady_clock::now();
    double timing = chrono::duration <double> (now-start_time).count();
    if (timing >= max_time or count > COUNT)
        return true;
    return false;
}

Solution memeticGeneticAlgo(PfspInstance instance, int N, float Pe, float Pc, float Pm, int COUNT, timing start_time, double max_time, vdouble &best_runtimes) {
    /*
     * Memetic genetic algorithm to find the best solution for the flow shop problem
     */
    int count = 1;  // record the nb of non improving consecutive generations
    int it = 0;
    vector<Solution> pop = generateFullRNDpop(instance, N);

    Solution best_sol = getBestChromosome(pop);
    Population E = eliteSelection(Pe, pop);  // elitist portion of the population
    Solution artificial_chromosome = WSMGS(instance, E);
    timing init_time = start_time;

    while (! terminationCriterion(start_time, max_time, count, COUNT)) {
//        cout << "     Generation " << it << " : " << best_sol.wct << " " << endl;

        Population offspring_pop;
        reproduction(instance, N, Pc, Pm, pop, offspring_pop, artificial_chromosome);

        // population updated
        pop = offspring_pop;
        E = eliteSelection(Pe, pop);
        artificial_chromosome = WSMGS(instance, E);
        Solution offspring_best_sol = getBestChromosome(pop);
        if (offspring_best_sol.wct < best_sol.wct) {
            best_sol = offspring_best_sol;
            count = 1;
        } else {
            count++;
        }
        if (!best_runtimes.empty())
            updateBestRuntimes(instance, best_runtimes, best_sol, init_time, max_time);
        it++;
    }


    return best_sol;

}


#endif //VERSION_C___MEMETICGA_H