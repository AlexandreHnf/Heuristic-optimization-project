//
// Created by Alexandre on 0027 27 avril 2021.
//

#ifndef VERSION_C___EXPERIMENTSASSIGNMENT2_H
#define VERSION_C___EXPERIMENTSASSIGNMENT2_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include "Utils.h"
#include "pfspinstance.h"
#include "flowshop.h"
#include "MemeticGA.h"
#include "tabu.h"

// ======== TABU PARAMETERS
#define TENURE 7

// ======== MGA PARAMETERS
#define PM 0.5
#define PC 1.0
#define PE 0.4
#define COUNT 10
#define POP_SIZE 50

#define MAX_TIME_50 0.1 //500
#define MAX_TIME_100 0.1 //350.0

/***************************************************************************/
// Some Unit tests of MGA and Tabu

void printPopulation(Population pop) {
    for (auto p : pop) {
        printSol(p);
    }
}

void testMutation() {
    Solution child = {{1,2,3,4,5,6,7,8,9}, 0};
    printSol(child);

    mutation(child, 0.2);
    printSol(child);
}

void testSorted(Population pop) {
    Population sorted = getSortedPopulation(pop);
    printPopulation(sorted);
}

void testEliteSelection(Population pop) {
    Population elite = eliteSelection(0.6, pop);
    printPopulation(elite);
}

void testGetAC(PfspInstance instance) {
    vvInt M = {
            {0,8,5,0,2},
            {8,2,4,0,1},
            {1,5,0,9,0},
            {4,0,2,6,3},
            {2,0,4,0,9}
    };
    Solution ac = getArtificialChromosome(instance, M, 5);
    printSol(ac);
}

void testWSMGS(PfspInstance instance, Population E) {
    Population sorted = getSortedPopulation(E);
    Solution ac = WSMGS(instance, sorted);
    printSol(ac);
}

void testCrossover(PfspInstance instance, Solution p1, Solution p2) {
    Solution ac = {{2,1,4,3,5}, 0};
    Solution son = crossover(instance, p1, p2, ac, 0.6);
    printSol(son);
    Solution daughter = crossover(instance, p2, p1, ac, 0.6);
    printSol(daughter);
}

void unitTestsMGA() {
    PfspInstance instance;
    instance.setNbJob(5);
    instance.setNbMac(5);
    //testMutation();

    Solution p1 = {{2,3,1,4,5}, 15};
    Solution p2 = {{2,1,5,3,4}, 34};
    Solution p3 = {{3,1,5,4,2}, 46};
    Solution p4 = {{5,2,4,3,1}, 38};
    Solution p5 = {{4,1,2,3,5}, 24};
    Population pop = {p1, p2, p3, p4, p5};

    testSorted(pop);
    testEliteSelection(pop);
    testGetAC(instance);
    testWSMGS(instance, pop);
    testCrossover(instance, p1, p3);
}

void testTabuInsert() {
    vInt pi = {1,2,3,4,5};
    for (int a = 0; a < 5; a++) {
        for (int b = 0; b < 5; b++) {
            if (a != b) {
                vInt tmp = pi;
                if (a < b)
                    insertRight(a, b, tmp);
                if (a > b)
                    insertLeft(a, b, tmp);
                cout << a << "," << b << endl;
                printVector(tmp, "");
            }
        }
    }
}

// =======================================================================================
// ==================================================
// =================== TEST MGA =====================
// ==================================================

void testMGASmallInstance(float Pe, float Pc, float Pm, int max_COUNT, int pop_size, double max_time) {
    PfspInstance instance_small;

    string filename_small = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\05_03_01.txt";
    if (! instance_small.readDataFromFile(filename_small) )
        return;
    cout << "ok parse" << endl;
    auto start_alg_time = chrono::steady_clock::now();
    Solution bs = memeticGeneticAlgo(instance_small, pop_size, Pe, Pc, Pm, COUNT, start_alg_time, max_time);
    cout << "Done in " << chrono::duration <double> (chrono::steady_clock::now()-start_alg_time).count() << " sec." << endl;
    printSol(bs);
}

void testMGAmediumInstance(float Pe, float Pc, float Pm, int max_COUNT, int pop_size, double max_time) {
    PfspInstance instance_medium;

//    string filename_medium = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\instances\\50_20_01";
    string filename_medium = "instances/50_20_01";
    if (! instance_medium.readDataFromFile(filename_medium) )
        return;
    cout << "ok parse" << endl;
    auto start_alg_time = chrono::steady_clock::now();
    Solution bs = memeticGeneticAlgo(instance_medium, pop_size, Pe, Pc, Pm, COUNT, start_alg_time, max_time);
    cout << "Done in " << chrono::duration <double> (chrono::steady_clock::now()-start_alg_time).count() << " sec." << endl;
    printSol(bs);
}

void testInstanceMGA(float Pm, float Pc, float Pe, int max_count, int pop_size, float max_time, string instance_file) {
    PfspInstance instance;
    if (! instance.readDataFromFile(instance_file) )
        return;
    srand ( time(NULL) );

    auto start_alg_time = chrono::steady_clock::now();
    Solution bs = memeticGeneticAlgo(instance, pop_size, Pe, Pc, Pm, max_count, start_alg_time, max_time);
    cout << instance_file << " done in " << chrono::duration <double> (chrono::steady_clock::now()-start_alg_time).count() << " sec." << endl;
    printSol(bs);
}


void testMGA() {
//    unitTestsMGA();

    float Pm = 0.5;
    float Pc = 1.0;
    float Pe = 0.4;
    int max_COUNT = 10;
    int pop_size = 50;
    double max_time = 50.0;

//    testMGASmallInstance(Pe, Pc, Pm, COUNT);
    testMGAmediumInstance(Pe, Pc, Pm, max_COUNT, pop_size, max_time);

}

// ==================================================
// =================== TEST TABU ====================
// ==================================================

void testTabuSmall(int tabu_tenure) {
    PfspInstance instance_small;

    string filename_small = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\05_03_01.txt";
    if (! instance_small.readDataFromFile(filename_small) )
        return;
    cout << "ok parse" << endl;
    auto start_alg_time = chrono::steady_clock::now();
    Solution bs = tabuSearch(instance_small, tabu_tenure, start_alg_time, 50);
    cout << "Done in " << chrono::duration <double> (chrono::steady_clock::now()-start_alg_time).count() << " sec." << endl;
    printSol(bs);
}

void testTabuMedium(int tabu_tenure) {
    PfspInstance instance_medium;

//    string filename_medium = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\instances\\50_20_01";
    string filename_medium = "instances/50_20_01";
    if (! instance_medium.readDataFromFile(filename_medium) )
        return;
    cout << "ok parse" << endl;
    auto start_alg_time = chrono::steady_clock::now();
    Solution bs = tabuSearch(instance_medium, tabu_tenure, start_alg_time, 50);
    cout << "Done in " << chrono::duration <double> (chrono::steady_clock::now()-start_alg_time).count() << " sec." << endl;
    printSol(bs);
}

void testInstanceTabu(int tabu_tenure, float max_time, string instance_file) {
    PfspInstance instance;
    if (! instance.readDataFromFile(instance_file) )
        return;
    srand ( time(NULL) );

    if (! instance.readDataFromFile(instance_file) )
        return;

    auto start_alg_time = chrono::steady_clock::now();
    Solution bs = tabuSearch(instance, tabu_tenure, start_alg_time, max_time);
    cout << instance_file << " done in " << chrono::duration <double> (chrono::steady_clock::now()-start_alg_time).count() << " sec." << endl;
    printSol(bs);
}

void testSLSOneInstance(PfspInstance instance, vvdouble &all_rpds, vvint &all_wcts, int best_known, int inst_nb, int run) {
    srand ( time(NULL) ); // RANDOM SEED per instance

    double max_time = MAX_TIME_50;
    if (instance.getNbJob() == 100)
        max_time = MAX_TIME_100;

    // run MGA on one instance
    auto start_ga_time = chrono::steady_clock::now();
    Solution solutionGA = memeticGeneticAlgo(instance, POP_SIZE, PE, PC, PM, COUNT, start_ga_time, max_time);
    all_wcts[inst_nb][run] = solutionGA.wct;
    all_rpds[inst_nb][run] = relativePercentageDeviation(solutionGA.wct, best_known);

    // run Tabu on one instance
    auto start_ts_time = chrono::steady_clock::now();
    Solution solutionTS = tabuSearch(instance, TENURE, start_ts_time, MAX_TIME_50);
    all_wcts[inst_nb][run+5] = solutionTS.wct;
    all_rpds[inst_nb][run+5] = relativePercentageDeviation(solutionTS.wct, best_known);
}


void testTabu() {

//    testTabuInsert();
//    testTabuSmall(4);
    testTabuMedium(7);
}

// ==================
// all experiments

void SLSalgosAllInstances(vvint &all_wcts, vvdouble &all_rpds, vvdouble &avg_rpds, vvint best_knowns, int run) {
    string base_filename = UFILE_INSTANCES;
    int inst_nb = 0;

    for (int i = 1; i >= 0; i--) { // all instances with 100 and then 50 jobs :
        for (int j = 0; j < 30; j++) { // nb of instances per job nb
            string instance_name = getInstanceName(i, j + 1);
            string inst_filename = base_filename + instance_name;
            cout << instance_name << endl;
            PfspInstance instance;
            instance.readDataFromFile(inst_filename);

            testSLSOneInstance(instance, all_rpds, all_wcts, best_knowns[i][j], inst_nb, run);

            avg_rpds[i][run] += all_rpds[inst_nb][run] / 30;        // GA result
            avg_rpds[i][run+5] += all_rpds[inst_nb][run+5] / 30;    // TS result
            inst_nb++;
        }
    }
}

void runAllExperimentsA2(vvint best_knowns) {
    // ======================================================
    // TEST GA+TS ON ALL INSTANCES (50 + 100)
    // ======================================================
    vvint all_wcts(60, vInt(10));
    vvdouble all_rpds(60, vdouble(10));
    vvdouble avg_rpds = {vdouble(10), vdouble(10)};

    for(int run=0; run < 5; run++) {
        cout << ">>>>>>>>>> Run " << run+1 << endl;
        SLSalgosAllInstances(all_wcts, all_rpds, avg_rpds, best_knowns, run);
    }

    string header = "instance,GA1,GA2,GA3,GA4,GA5,TS1,TS2,TS3,TS4,TS5\n";
    vector<string> filenames = {UFILE_SLS_ALL_INST_WCT, UFILE_SLS_ALL_INST_RPD};

    writeAllInstancesResToFile(all_wcts, all_rpds, header, filenames);
    writeAlgosStatsToFile(UFILE_SLS_AVG_RPDS, avg_rpds, header);
}

#endif //VERSION_C___EXPERIMENTSASSIGNMENT2_H
