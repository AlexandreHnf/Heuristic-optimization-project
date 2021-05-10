//
// Created by Alexandre on 0005 5 mai 2021.
//

#ifndef VERSION_C___UTILS_H
#define VERSION_C___UTILS_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

typedef chrono::steady_clock::time_point timing;

// ========== FOR WINDOWS
#define W "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\"

#define FILE_INSTANCES W "instances\\"
#define FILE_INST_SMALL W "small instance\\05_03_01"
#define FILE_BEST_KNOWN_SOLS W "instances\\bestSolutions.txt"

// ASSIGNMENT 1
#define FILE_II_ALL_INST_WCT W "results\\II_WCT_test.txt"
#define FILE_II_ALL_INST_RPD W "results\\II_RPD_test.txt"
#define FILE_VND_ALL_INST_WCT W "results\\VND_WCT_test.txt"
#define FILE_VND_ALL_INST_RPD W "results\\VND_RPD_test.txt"

#define FILE_II_AVG_RPDS W "results\\II_avgRPDs_test.txt"
#define FILE_II_AVG_CTS W "results\\II_avgCTs_test.txt"
#define FILE_VND_AVG_RPDS W "results\\VND_avgRPDs_test.txt"
#define FILE_VND_AVG_CTS W "results\\VND_avgCTs_test.txt"

// ASSIGNMENT 2
// Results MGA

// Results Tabu

// ========== FOR UBUNTU

#define UFILE_INSTANCES "instances/"
#define UFILE_INST_SMALL "small/05_03_01";
#define UFILE_BEST_KNOWN_SOLS "instances/bestSolutions.txt";

// ASSIGNMENT 1
// Results Iterative Improvement
#define UFILE_II_ALL_INST_WCT "results/II_WCT_results.csv"
#define UFILE_II_ALL_INST_RPD "results/II_RPD_results.csv"
#define UFILE_II_AVG_RPDS "results/II_avgRPDs_results.csv"
#define UFILE_II_AVG_CTS "results/II_avgCTs_results.csv"
// Results Variable Neighbourhood Descent
#define UFILE_VND_ALL_INST_WCT "results/VND_WCT_results.csv"
#define UFILE_VND_ALL_INST_RPD "results/VND_RPD_results.csv"
#define UFILE_VND_AVG_RPDS "results/VND_avgRPDs_results.csv"
#define UFILE_VND_AVG_CTS "results/VND_avgCTs_results.csv"
#define UFILE_VND_PI "results/VND_PI_results.csv"

// ASSIGNMENT 2
// Results MGA
#define UFILE_SLS_ALL_INST_WCT "results/SLS_WCT_results.csv"
#define UFILE_SLS_ALL_INST_RPD "results/SLS_RPD_results.csv"
#define UFILE_SLS_ALL_INST_AVG_RPDS "results/SLS_avgrunsRPDs_results.csv"
#define UFILE_SLS_AVG_RPDS "results/SLS_avgRPDs_results.csv"

// Results RTDs
#define UFILE_SLS_RTD_1 "results/SLS_RTD_1.csv"
#define UFILE_SLS_RTD_2 "results/SLS_RTD_2.csv"

// ==========================================================================

template <typename T>
std::ostream & operator << (std::ostream & os, const std::vector<T> & vec)
{
    for(auto elem : vec) {
        os << elem << ", ";
    }
    return os;
}

string getInstanceName(int i, int j) {
    // i = represents instance with 50 or 100 jobs
    // j = the instance number
    string instance_name = "";
    if (i == 0) {
        instance_name += "50_20_";
    } else {
        instance_name += "100_20_";
    }
    string instance_nb = "";
    if (j < 10) {
        instance_nb = "0" + to_string(j);
    } else {
        instance_nb += to_string(j);
    }
    return instance_name + instance_nb;
}

double relativePercentageDeviation(int wct, int best_known) {
    /*
    Computes the relative percentage deviation of the algorithm k on the instance i
    algorithm k = a combination of different mode
    for example : --random --firstImprovement --tranpose
    */
    return 100 * (((double) wct - (double) best_known) / (double) best_known);
}

template <typename T>
void writeAllInstancesResToFile(vector<vector<T>> all_res, string header, string filename) {

    int nb_algos = all_res[0].size();
    ofstream myfile;
    myfile.open(filename);
//    myfile << "sep=,\n";
    myfile << header;

    string line;
    int global_index = 0;
//		for (int inst_nb_index = 0; inst_nb_index < 2; inst_nb_index++) { // 0 = instances 50, 1 = instances 100
    for (int inst_nb_index = 1; inst_nb_index >= 0; inst_nb_index--) { // 0 = instances 50, 1 = instances 100
        for (int i = 0; i < 30; i++) {

            line = getInstanceName(inst_nb_index, i + 1) + ","; // +1 bc it starts at 01
            for (int a = 0; a < nb_algos; a++) {
                line += to_string(all_res[global_index][a]);

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
    cout << "written to " << filename << ": ok" << endl;
}

void writeRTDToFile(vector<vector<double>> all_timings, string header, string filename) {
    int nb_rows = all_timings.size();
    cout << "nb rows to write : " << nb_rows << endl;
    int nb_percentages = 10; // nb of percentages (0.1,0.5... ga + ts = 10)
    ofstream myfile;
    myfile.open(filename);
    myfile << header;
    string line;
    for (int i = 0; i < nb_rows; i++) {
        line = "";
        for (int p = 0; p < nb_percentages; p++) {
            line += to_string(all_timings[i][p]);
            if (p < nb_percentages-1) {
                line += ",";
            } else {
                line += "\n";
            }
        }
        myfile << line;
    }
    myfile.close();
    cout << "written to " << filename << ": ok" << endl;
}

void updateBestRuntimes(PfspInstance instance, vector<double> &best_runtimes, Solution best_sol, timing start_time, double max_time) {
    for (int i = 0; i < instance.getNbBestSols(); i++) {
        if (best_runtimes[i] == -1.0) {
            int close_sol = instance.getClosestSolution(i);
            int diff = best_sol.wct - close_sol;
            // if close enough to the target solution
            if (close_sol >= best_sol.wct or (diff > 0 and diff < 0.03*close_sol/100)) {
                best_runtimes[i] = chrono::duration <double> (chrono::steady_clock::now()-start_time).count();
                cout << "close sol " << i << " found in : " << best_runtimes[i] << " sec." << endl;
            }
            // if max time reached without finding sufficiently high sol
            if (chrono::duration <double> (chrono::steady_clock::now()-start_time).count() >= max_time)
                best_runtimes[i] = max_time;
        }
    }
}


#endif //VERSION_C___UTILS_H
