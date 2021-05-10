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
#include <thread>         // std::thread
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
#define COUNT 1000
#define POP_SIZE 20

#define MAX_TIME_50 150.0  //0.1
#define MAX_TIME_100 350.0 // 0.1

// ======== RTD PARAMETERS
#define NB_ITERATIONS 5
#define NB_INSTANCES 2
#define MAX_TIMING 200.0

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
    vdouble best_runtimes(5, -1.0);
    Solution bs = memeticGeneticAlgo(instance_small, pop_size, Pe, Pc, Pm, COUNT, start_alg_time, max_time, best_runtimes);
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
    vdouble best_runtimes(5, -1.0);
    Solution bs = memeticGeneticAlgo(instance_medium, pop_size, Pe, Pc, Pm, COUNT, start_alg_time, max_time, best_runtimes);
    cout << "Done in " << chrono::duration <double> (chrono::steady_clock::now()-start_alg_time).count() << " sec." << endl;
    printSol(bs);
}

void testInstanceMGA(float Pm, float Pc, float Pe, int max_count, int pop_size, float max_time, string instance_file) {
    PfspInstance instance;
    instance.setBestSolutions(595260);
    if (! instance.readDataFromFile(instance_file) )
        return;
    srand ( time(NULL) );

    auto start_alg_time = chrono::steady_clock::now();
    vdouble best_runtimes(5, -1.0);
    Solution bs = memeticGeneticAlgo(instance, pop_size, Pe, Pc, Pm, max_count, start_alg_time, max_time, best_runtimes);
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
    vdouble best_runtimes(5, -1.0);
    Solution bs = tabuSearch(instance_small, tabu_tenure, start_alg_time, 50, best_runtimes);
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
    vdouble best_runtimes(5, -1.0);
    Solution bs = tabuSearch(instance_medium, tabu_tenure, start_alg_time, 50, best_runtimes);
    cout << "Done in " << chrono::duration <double> (chrono::steady_clock::now()-start_alg_time).count() << " sec." << endl;
    printSol(bs);
}

void testInstanceTabu(int tabu_tenure, float max_time, string instance_file) {
    PfspInstance instance;
    instance.setBestSolutions(595260);
    if (! instance.readDataFromFile(instance_file) )
        return;
    srand ( time(NULL) );

    if (! instance.readDataFromFile(instance_file) )
        return;

    auto start_alg_time = chrono::steady_clock::now();
    vdouble best_runtimes(5, -1.0);
    Solution bs = tabuSearch(instance, tabu_tenure, start_alg_time, max_time, best_runtimes);
    cout << instance_file << " done in " << chrono::duration <double> (chrono::steady_clock::now()-start_alg_time).count() << " sec." << endl;
    printSol(bs);
}

void testSLSOneInstance(PfspInstance instance, vvdouble &all_rpds, vvint &all_wcts, int best_known, int inst_nb, int run) {

    double max_time = MAX_TIME_50;
    if (instance.getNbJob() == 100)
        max_time = MAX_TIME_100;

    // run MGA on one instance
    auto start_ga_time = chrono::steady_clock::now();
    vdouble best_runtimes(5, -1.0);
    Solution solutionGA = memeticGeneticAlgo(instance, POP_SIZE, PE, PC, PM, COUNT, start_ga_time, max_time, best_runtimes);
    all_wcts[inst_nb][run] = solutionGA.wct;
    all_rpds[inst_nb][run] = relativePercentageDeviation(solutionGA.wct, best_known);

    // run Tabu on one instance
    auto start_ts_time = chrono::steady_clock::now();
    vdouble best_runtimes2(5, -1.0);
    Solution solutionTS = tabuSearch(instance, TENURE, start_ts_time, max_time, best_runtimes2);
    all_wcts[inst_nb][run+5] = solutionTS.wct;
    all_rpds[inst_nb][run+5] = relativePercentageDeviation(solutionTS.wct, best_known);
}


void testTabu() {

//    testTabuInsert();
//    testTabuSmall(4);
    testTabuMedium(7);
}

// ==================================================
// ============== ALL EXPERIMENTS  ==================
// ==================================================

void SLSalgosAllInstances(vvint &all_wcts, vvdouble &all_rpds, vvdouble &avg_rpds, vvint best_knowns, int run) {
    srand ( time(NULL) ); // random seed per run, but same for both GA and TS
    string base_filename = UFILE_INSTANCES;
    int inst_nb = 0;

    for (int i = 1; i >= 0; i--) { // all instances with 100 and then 50 jobs :
        for (int j = 0; j < 30; j++) { // nb of instances per job nb
            string instance_name = getInstanceName(i, j + 1);
            string inst_filename = base_filename + instance_name;
            cout << run+1 << " : " << instance_name << endl;
            PfspInstance instance;
            instance.readDataFromFile(inst_filename);

            testSLSOneInstance(instance, all_rpds, all_wcts, best_knowns[i][j], inst_nb, run);

            avg_rpds[i][run] += all_rpds[inst_nb][run] / 30;        // GA result
            avg_rpds[i][run+5] += all_rpds[inst_nb][run+5] / 30;    // TS result
            inst_nb++;
        }
    }
}

vvdouble getAvgRunsRpds(vvdouble avg_rpds) {
    vvdouble avg_runs_rpds(avg_rpds.size());
    for(int i = 0; i < avg_rpds.size(); i++) {
        double avg_ga_rpd = 0;
        double avg_ts_rpd = 0;
        for (int j = 0; j < 5; j++) {
            avg_ga_rpd += avg_rpds[i][j] / 5;
            avg_ts_rpd += avg_rpds[i][j+5] / 5;
        }
        avg_runs_rpds[i] = {avg_ga_rpd, avg_ts_rpd};
    }
    return avg_runs_rpds;
}

void runAllExperimentsA2(vvint best_knowns) {
    // ======================================================
    // TEST GA+TS ON ALL INSTANCES (50 + 100)
    // ======================================================
    vvint all_wcts(60, vInt(10));
    vvdouble all_rpds(60, vdouble(10));
    vvdouble avg_rpds = {vdouble(10), vdouble(10)};

    auto start_runs_time = chrono::steady_clock::now();

    // start threads
    vector<thread> runs(5);
    for (int i = 0; i < 5; i++)
        runs[i] = thread(SLSalgosAllInstances,ref(all_wcts), ref(all_rpds), ref(avg_rpds), best_knowns, i);
    for (int i = 0; i < 5; i++)
        runs[i].join(); // synchronize threads, pauses until all runs finish:

    cout <<"5 runs done in " << chrono::duration <double> (chrono::steady_clock::now()-start_runs_time).count() << " sec." << endl;

    string header = "instance,GA1,GA2,GA3,GA4,GA5,TS1,TS2,TS3,TS4,TS5\n";

    writeAllInstancesResToFile(all_wcts, header, UFILE_SLS_ALL_INST_WCT);
    writeAllInstancesResToFile(all_rpds, header, UFILE_SLS_ALL_INST_RPD);
    vvdouble avg_runs_rpds = getAvgRunsRpds(all_rpds);
    string header_avg = "instance,GA,TS\n";
    writeAllInstancesResToFile(avg_runs_rpds, header_avg, UFILE_SLS_ALL_INST_AVG_RPDS);
    writeAlgosStatsToFile(UFILE_SLS_AVG_RPDS, avg_rpds, header);
}

// ==================================================
// ============ RUN-TIME DISTRIBUTIONS ==============
// ==================================================

void RTDoneInstance(PfspInstance instance, vvdouble &all_timings, double max_time, int run) {
    auto start_ga_time = chrono::steady_clock::now();
    vdouble best_runtimes1(5, -1.0);
    Solution solutionGA = memeticGeneticAlgo(instance, POP_SIZE, PE, PC, PM, INT_MAX, start_ga_time, max_time, best_runtimes1);

    // run Tabu on one instance
    auto start_ts_time = chrono::steady_clock::now();
    vdouble best_runtimes2(5, -1.0);
    Solution solutionTS = tabuSearch(instance, TENURE, start_ts_time, max_time, best_runtimes2);

    best_runtimes1.insert( best_runtimes1.end(), best_runtimes2.begin(), best_runtimes2.end() );
    all_timings[run] = best_runtimes1;
}

void runAllRTDexperiments(vvint best_knowns) {
    vector<string> filenames = {UFILE_SLS_RTD_1, UFILE_SLS_RTD_2};

    string header = "BK,GA01,GA05,GA1,GA2,BK2,TS01,TS05,TS1,TS2\n";
    for (int inst = 0; inst < NB_INSTANCES; inst++) {
        cout << "INSTANCE " << inst+1 << endl;
        auto start_run_time = chrono::steady_clock::now();
        PfspInstance instance;
        instance.readDataFromFile("instances/" + getInstanceName(0, inst+1));
        instance.setBestSolutions(best_knowns[0][inst]);
        vvdouble all_timings(NB_ITERATIONS, vdouble(10, -1));

        // start threads
        vector<thread> runs(NB_ITERATIONS);
        for (int i = 0; i < NB_ITERATIONS; i++)
            runs[i] = thread(RTDoneInstance, instance, ref(all_timings), MAX_TIMING, i);
        for (int i = 0; i < NB_ITERATIONS; i++)
            runs[i].join(); // synchronize threads, pauses until all runs finish:

        cout <<"Instance " << inst+1 << " done in " << chrono::duration <double> (chrono::steady_clock::now()-start_run_time).count() << " sec." << endl;
        writeRTDToFile(all_timings, header, filenames[inst]);
    }
}

#endif //VERSION_C___EXPERIMENTSASSIGNMENT2_H

