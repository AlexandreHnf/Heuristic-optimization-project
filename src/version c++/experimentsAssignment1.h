//
// Created by Alexandre on 0027 27 avril 2021.
//

#ifndef VERSION_C___EXPERIMENTSASSIGNMENT1_H
#define VERSION_C___EXPERIMENTSASSIGNMENT1_H



#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include "pfspinstance.h"
#include "flowshop.h"
#include "Utils.h"

using namespace std;
typedef vector<double> vdouble;
typedef vector<vector<int>> vvint;
typedef vector<vector<double>> vvdouble;

/***************************************************************************/

void writeAllInstancesResToFile(vvint all_wcts, vvdouble all_rpds, string header, vector<string> filenames) {

    int nb_algos = all_wcts[0].size();
    for (int k = 0; k < 2; k++) {
        ofstream myfile;
        myfile.open(filenames[k]);
        myfile << "sep=,\n";
        myfile << header;

        string line;
        int global_index = 0;
//		for (int inst_nb_index = 0; inst_nb_index < 2; inst_nb_index++) { // 0 = instances 50, 1 = instances 100
        for (int inst_nb_index = 1; inst_nb_index >= 0; inst_nb_index--) { // 0 = instances 50, 1 = instances 100
            for (int i = 0; i < 30; i++) {

                line = getInstanceName(inst_nb_index, i + 1) + ","; // +1 bc it starts at 01
                for (int a = 0; a < nb_algos; a++) {
                    if (k == 0) { // if first file : WCTs
                        line += to_string(all_wcts[global_index][a]);
                    } else { // if second file : RPDs
                        line += to_string(all_rpds[global_index][a]);
                    }

                    if (a < nb_algos - 1) {
                        line += ",";
                    } else {
                        line += "\n";
                    }
                }
                myfile << line;
                global_index++;
            }
        }
        myfile.close();
    }
    cout << "written to files : ok" << endl;
}

void writeAlgosStatsToFile(string filename, vvdouble stats, string header) {
    // stats = either average computation times, either average relative percentage deviations
    ofstream myfile;
    myfile.open(filename);
    myfile << "sep=,\n";
    myfile << header;
    string line;
    for (int inst_nb_index = 0; inst_nb_index < 2; inst_nb_index++) { // 0 = instances 50, 1 = instances 100
        line = "50,";
        if (inst_nb_index == 1)
            line = "100,";
        for (int a = 0; a < stats[0].size(); a++) {
            line += to_string(stats[inst_nb_index][a]);
            if (a < stats[0].size() - 1) {
                line += ",";
            } else {
                line += "\n";
            }
        }
        myfile << line;
    }
    myfile.close();
    cout << "written to files : ok" << endl;
}

/***************************************************************************/

void testAllAlgosII(PfspInstance instance, vdouble &computation_times, vdouble &RPD, int best_known, vvint &all_wcts, int inst_nb) {
    /*
    1)  R - FI - T          7)  S - FI - T
    2)  R - FI - E          8)  S - FI - E
    3)  R - FI - I          9)  S - FI - I
    4)  R - BI - T          10) S - BI - T
    5)  R - BI - E          11) S - BI - E
    6)  R - BI - I          12) S - BI - I
     12 algos = RFT,RFE,RFI,RBT,RBE,RBI,SFT,SFE,SFI,SBT,SBE,SBI
    */
    srand ( time(NULL) ); // RANDOM SEED per instance
//    cout << "=== TEST ALL ALGOS (12) ===" << endl;
    vector<vector<string>> modes = {vector<string>{"R", "SRZ"}, vector<string>{"FI", "BI"}, vector<string>{"T", "E", "I"}};
    int algo_id = 0;
    Solution start_sol_R = generateInitialSolution(modes[0][0], instance);
    Solution start_sol_SRZ = generateInitialSolution(modes[0][1], instance);
    vector<Solution> init_sols = {start_sol_R, start_sol_SRZ};
//    cout << "---------------------------" << endl;

    auto start = chrono::steady_clock::now();
    Solution solution;
    for (int s = 0; s < 2; s++) {
        for (int p = 0; p < 2; p++) {
            for (int n = 0; n < 3; n++) {
                auto start_alg_time = chrono::steady_clock::now();
                solution = iterativeImprovement(modes[1][p], modes[2][n], instance, init_sols[s]);
                all_wcts[inst_nb].push_back(solution.wct); // save wct of the algo on this instance
//                cout << "ALGO " << modes[0][s] << "-" << modes[1][p] << "-" << modes[2][n] << ": " << endl;
//                printSol(solution);
                auto end_alg_time = chrono::steady_clock::now();
                computation_times[algo_id] = chrono::duration <double> (end_alg_time-start_alg_time).count();
                RPD[algo_id] = relativePercentageDeviation(solution.wct, best_known);
//                cout << "==> " << chrono::duration <double> (end_alg_time-start_alg_time).count() << "sec" << endl;
//                cout << "-------------------" << endl;
                algo_id++;
            }
        }
    }
    auto end = chrono::steady_clock::now();
    cout << "Done in " << chrono::duration <double> (end-start).count() << "sec" << endl;
//    cout << "====================================" << endl;
}



/***************************************************************************/

void IItestAllInstances(vvint & all_wcts, vvdouble & all_rpds, vvdouble & avg_CTs, vvdouble & avg_RPDs, vvint best_knowns) {
    /*
    Run the 12 algos on each instance
     => 12 algos on 30 instances of 50 jobs and then on 30 instances of 100 jobs
     result = an average relative percentage deviation per algo + average computation time of each algo
     and one average per job nb so 50 and 100 => 2 averages per algo => 24 in total (12 algos)
     12 algos = RFT,RFE,RFI,RBT,RBE,RBI,SFT,SFE,SFI,SBT,SBE,SBI
    */

    // string base_filename = FILE_INSTANCES;
    string base_filename = UFILE_INSTANCES;
    string instance_name;

    int inst_nb = 0;

    auto start = chrono::steady_clock::now();
    // all instances with 50 and then 100 jobs :

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 30; j++) { // nb of instances per job nb
            instance_name = getInstanceName(i, j + 1);
            string inst_filename = base_filename + instance_name;
            cout << "Instance " << instance_name << ": " << endl;
            PfspInstance instance;
            instance.readDataFromFile(inst_filename);

            vdouble computation_times(12);
            vdouble RPD(12);
            testAllAlgosII(instance, computation_times, RPD, best_knowns[i][j], all_wcts, inst_nb);
            for (int k = 0; k < 12; k++) {
                avg_CTs[i][k] += computation_times[k] / 30;
                avg_RPDs[i][k] += RPD[k] / 30;
                all_rpds[inst_nb].push_back(RPD[k]);
            }
            inst_nb++;
        }
    }
    auto end = chrono::steady_clock::now();
    double timing = chrono::duration <double> (end-start).count();

    cout << "END ========================= " << endl;
    cout << "All II instances done in " << timing << "sec" << endl;
//    cout << "average CT 50 jobs : " << avg_CTs[0] << endl;
//    cout << "average CT 100 jobs : " << avg_CTs[1] << endl;
//    cout << "average RPD 50 jobs : " << avg_RPDs[0] << endl;
//    cout << "average RPD 100 jobs : " << avg_RPDs[1] << endl;
}

/***************************************************************************/



void testAllAlgosVND(PfspInstance instance, vdouble &computation_times, vdouble &RPD, vvint &all_wcts, int best_known, int inst_nb) {
    /*
     Test all combination of VND-PFSP on 1 instance
     result = a relative percentage deviation per algo + computation time per algo
     on the given instance.

     1) R-T-E-I   2) R-T-I-E   3) S-T-E-I   4) S-T-I-E
    */
//    cout << "=== TEST ALL VND (4) ===" << endl;
    vector<vector<string>> modes = {vector<string>{"R", "SRZ"}, vector<string>{"T", "E", "I"}, vector<string>{"T", "I", "E"}};
    int algo_id = 0;
    Solution start_sol_R = generateInitialSolution(modes[0][0], instance);
    Solution start_sol_SRZ = generateInitialSolution(modes[0][1], instance);
    vector<Solution> init_sols = {start_sol_R, start_sol_SRZ};

    auto start = chrono::steady_clock::now();
    Solution solution;
    for (int s = 0; s < 2; s++) { // starting modes (Random or SRZ)
        for (int n = 1; n < 3; n++) { // neighbourhoods sets (neighbourhoods 1, neighbourhoods 2)
            auto start_alg_time = chrono::steady_clock::now();
//            cout << "ALGO " << modes[0][s] << "-" << modes[n][0] << "-" <<
//                 modes[n][1] << "-" << modes[n][2] << endl;
            solution = variableNeighbourhoodDescent(modes[n], instance, init_sols[s]);
            all_wcts[inst_nb].push_back(solution.wct); // save wct of the algo on this instance
//            printSol(solution);
            auto end_alg_time = chrono::steady_clock::now();
            computation_times[algo_id] = chrono::duration <double> (end_alg_time-start_alg_time).count();
            RPD[algo_id] = relativePercentageDeviation(solution.wct, best_known);
//            cout << "==> " << chrono::duration <double> (end_alg_time-start_alg_time).count() << "sec" << endl;
            algo_id++;
        }
    }
    auto end = chrono::steady_clock::now();
    double timing = chrono::duration <double> (end-start).count();
    cout << "4 VND Done in " << timing << "sec" << endl;
}

void VNDtestAllInstances(vvint & all_wcts, vvdouble & all_rpds, vvdouble & avg_CTs, vvdouble & avg_RPDs, vvint best_knowns) {
    /*
     Test all combination of VND-PFSP on all instances
     result = an average relative percentage deviation per algo + average computation time of each algo
     and one average per job nb so 50 and 100 => 2 averages per algo => 8 in total (4 algos)
    */

    // string base_filename = FILE_INSTANCES;
    string base_filename = UFILE_INSTANCES;
    string instance_name;

    int inst_nb = 0;

    auto start = chrono::steady_clock::now();
    // all instances with 50 and then 100 jobs :

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 30; j++) { // nb of instances per job nb
            instance_name = getInstanceName(i, j + 1);
            string inst_filename = base_filename + instance_name;
            cout << "Instance " << instance_name << ": " << endl;
            PfspInstance instance;
            instance.readDataFromFile(inst_filename);

            vdouble computation_times(4);
            vdouble RPD(4);
            testAllAlgosVND(instance, computation_times, RPD, all_wcts, best_knowns[i][j], inst_nb);
            for (int k = 0; k < 4; k++) {
                avg_CTs[i][k] += computation_times[k] / 30;
                avg_RPDs[i][k] += RPD[k] / 30;
                all_rpds[inst_nb].push_back(RPD[k]);
            }
            inst_nb++;
        }
    }
    auto end = chrono::steady_clock::now();
    double timing = chrono::duration <double> (end-start).count();

    cout << "END ========================= " << endl;
    cout << "All VND instances done in " << timing << "sec" << endl;
//    cout << "average CT 50 jobs : " << avg_CTs[0] << endl;
//    cout << "average CT 100 jobs : " << avg_CTs[1] << endl;
//    cout << "average RPD 50 jobs : " << avg_RPDs[0] << endl;
//    cout << "average RPD 100 jobs : " << avg_RPDs[1] << endl;
}


vvdouble VNDpercentageImprovement(vvdouble avg_RPDs_VND, vvdouble avg_RPDs, vector<int> single_neighbour_indices) {
    vvdouble PI_VND = {vdouble(4), vdouble(4)};
    for (int i = 0; i < avg_RPDs.size(); i++) { // job nb
        for (int j = 0; j < avg_RPDs_VND[0].size(); j++) { // VND algos
            // relativePercentageDeviation(wct, best_known) => between relative percentage deviations
            // if positive, then there is an improvement of VND over II for the given neighbourhood rule (insert or exchange)
            double a = avg_RPDs[i][single_neighbour_indices[j]];
            double b = avg_RPDs_VND[i][j];
            PI_VND[i][j] = 100*((a-b)/b); // relative percentage deviation
        }
    }
    return PI_VND;
}

/***************************************************************************/

void testInstanceII(string instance_filename, string init_rule, string pivoting_rule, string neigh_rule) {
    // TEST OF ITERATIVE IMPROVEMENT ALGO ON ONE INSTANCE with given parameters
    cout << "========= TEST II on instance " << instance_filename << " ==========" << endl;
    PfspInstance instance;

    if (! instance.readDataFromFile(instance_filename) )
        return;

    srand ( time(NULL) );
    Solution start_sol = generateInitialSolution(init_rule, instance);
    /* Compute the WCT of this solution */
    int wct = instance.computeWCT(start_sol.sol);
    cout << "WCT start sol : " << wct << endl;

    auto start_alg_time = chrono::steady_clock::now();
    Solution solution = iterativeImprovement(pivoting_rule, neigh_rule, instance, start_sol);
    printSol(solution);
    auto end_alg_time = chrono::steady_clock::now();
    cout << "Time spent : " << chrono::duration <double> (end_alg_time-start_alg_time).count() << " sec" << endl;
}

void testInstanceVND(string instance_filename, string init_rule, vector<string> neighbour_rules) {
    // TEST OF VARIABLE NEIGHBOURHOOD DESCENT ALGO ON ONE INSTANCE with given parameters
    cout << "========= TEST VND on instance " << instance_filename << " ==========" << endl;
    PfspInstance instance;

    if (! instance.readDataFromFile(instance_filename) )
        return;

    srand ( time(NULL) );
    Solution start_sol = generateInitialSolution(init_rule, instance);
    /* Compute the WCT of this solution */
    int wct = instance.computeWCT(start_sol.sol);
    cout << "WCT start sol : " << wct << endl;

    auto start_alg_time = chrono::steady_clock::now();
    Solution solution = variableNeighbourhoodDescent(neighbour_rules, instance, start_sol);
    printSol(solution);
    auto end_alg_time = chrono::steady_clock::now();
    cout << "Time spent : " << chrono::duration <double> (end_alg_time-start_alg_time).count() << " sec" << endl;

}

void runAllExperimentsA1(vvint best_knowns) {
    // ======================================================
    // TEST ITERATIVE IMPROVEMENT ON ALL INSTANCES (50 + 100)
    // ======================================================
    vvint all_wcts(60); // 60 = nb of instances. Each sublist = 12 wct (12 algos)
    vvdouble all_rpds(60); // 60 = nb of instances. Each sublist = 12 wct (12 algos)
    vvdouble avg_CTs = {vdouble(12), vdouble(12)};
    vvdouble avg_RPDs = {vdouble(12), vdouble(12)};
    IItestAllInstances(all_wcts, all_rpds, avg_CTs, avg_RPDs, best_knowns);
    string header = "instance,RFT,RFE,RFI,RBT,RBE,RBI,SFT,SFE,SFI,SBT,SBE,SBI\n";
    vector<string> filenames = {UFILE_II_ALL_INST_WCT, UFILE_II_ALL_INST_RPD};
    writeAllInstancesResToFile2(all_wcts, header, UFILE_II_ALL_INST_WCT);
    writeAllInstancesResToFile2(all_rpds, header, UFILE_II_ALL_INST_RPD);
    vector<string> filenamesII_avg = {UFILE_II_AVG_RPDS, UFILE_II_AVG_CTS};
    writeAlgosStatsToFile(filenamesII_avg[0], avg_RPDs, header);
    writeAlgosStatsToFile(filenamesII_avg[1], avg_CTs, header);

    // ======================================================
    // TEST VARIABLE NEIGHBOURHOOD DESCENT ON ALL INSTANCES (50 + 100)
    // ======================================================
    vvint all_wcts_VND(60); // 60 = nb of instances. Each sublist = 4 wct (4 algos)
    vvdouble all_rpds_VND(60); // 60 = nb of instances. Each sublist = 4 wct (4 algos)
    vvdouble avg_CTs_VND = {vdouble(4), vdouble(4)};
    vvdouble avg_RPDs_VND = {vdouble(4), vdouble(4)};
    VNDtestAllInstances(all_wcts_VND, all_rpds_VND, avg_CTs_VND, avg_RPDs_VND, best_knowns);
    string header_vnd = "instance,RTEI,RTIE,STEI,STIE\n";
    vector<string> filenamesVND = {UFILE_VND_ALL_INST_WCT, UFILE_VND_ALL_INST_RPD};
    writeAllInstancesResToFile2(all_wcts_VND, header_vnd, UFILE_VND_ALL_INST_WCT);
    writeAllInstancesResToFile2(all_rpds_VND, header_vnd, UFILE_VND_ALL_INST_RPD);
    vector<string> filenamesVND_stats = {UFILE_VND_AVG_RPDS, UFILE_VND_AVG_CTS, UFILE_VND_PI};
    writeAlgosStatsToFile(filenamesVND_stats[0], avg_RPDs_VND, header_vnd);
    writeAlgosStatsToFile(filenamesVND_stats[1], avg_CTs_VND, header_vnd);

    // VND : percentage improvement over a single neighbourhood, in particular: exchange and insert
    vector<int> single_neighbour_indices = {1, 2, 7, 8}; // RFE,RFI,SFE,SFI
    vvdouble PI_VND = VNDpercentageImprovement(avg_RPDs_VND, avg_RPDs, single_neighbour_indices); // compare RTEI,RTIE,STEI,STIE with RFE,RFI,SFE,SFI
    writeAlgosStatsToFile(filenamesVND_stats[2], PI_VND, header_vnd);
}



#endif //VERSION_C___EXPERIMENTSASSIGNMENT1_H