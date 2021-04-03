#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include "pfspinstance.h"
#include "flowshop.h"

// FOR WINDOWS
#define W "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\"
#define FILE_II_ALL_INST_WCT W "results\\II_WCT_test.txt"
#define FILE_II_ALL_INST_RPD W "results\\II_RPD_test.txt"
#define FILE_VND_ALL_INST_WCT W "results\\VND_WCT_test.txt"
#define FILE_VND_ALL_INST_RPD W "results\\VND_RPD_test.txt"

#define FILE_INSTANCES W "instances\\"
#define FILE_INST_SMALL W "small instance\\05_03_01";
#define FILE_INST_MEDIUM W "instances\\50_20_01";
#define FILE_INST_BIG W "instances\\100_20_01";
#define FILE_BEST_KNOWN_SOLS W "instances\\bestSolutions.txt";
#define FILE_II_AVG_RPDS W "results\\II_avgRPDs_test.txt"
#define FILE_II_AVG_CTS W "results\\II_avgCTs_test.txt"
#define FILE_VND_AVG_RPDS W "results\\VND_avgRPDs_test.txt"
#define FILE_VND_AVG_CTS W "results\\VND_avgCTs_test.txt"

// FOR UBUNTU
#define UFILE_II_ALL_INST_WCT "results/II_WCT_test.txt"
#define UFILE_II_ALL_INST_RPD "results/II_RPD_test.txt"
#define UFILE_VND_ALL_INST_WCT "results/VND_WCT_test.txt"
#define UFILE_VND_ALL_INST_RPD "results/VND_RPD_test.txt"

#define UFILE_INSTANCES "instances/"
#define UFILE_INST_SMALL "small instance/05_03_01";
#define UFILE_INST_MEDIUM "instances/50_20_01";
#define UFILE_INST_BIG "instances/100_20_01";
#define UFILE_BEST_KNOWN_SOLS "instances/bestSolutions.txt";
#define UFILE_II_AVG_RPDS "results/II_avgRPDs_test.txt"
#define UFILE_II_AVG_CTS "results/II_avgCTs_test.txt"
#define UFILE_VND_AVG_RPDS "results/VND_avgRPDs_test.txt"
#define UFILE_VND_AVG_CTS "results/VND_avgCTs_test.txt"

using namespace std;
typedef vector<double> vdouble;
typedef vector<vector<int>> vvint;
typedef vector<vector<double>> vvdouble;

template <typename T>
std::ostream & operator << (std::ostream & os, const std::vector<T> & vec)
{
    for(auto elem : vec) {
        os << elem << ", ";
    }
    return os;
}


void testNeighbourBestImprovement(PfspInstance instance, Solution sol) {
    cout << "--------------------------------" << endl;
    cout << "BEST IMPROVEMENT NEIGHBOURHOODS" << endl;

    cout << "transpose : " << endl;
    Solution st = getBestTransposeNeighbour(sol, instance, "BI");
    printSol(st);

    cout << "exchange : " << endl;
    Solution se = getBestExchangeNeighbour(sol, instance, "BI");
    printSol(se);

    cout << "insertion : " << endl;
    Solution si = getBestInsertionNeighbour(sol, instance, "BI");
    printSol(si);
}

void testNeighbourFirstImprovement(PfspInstance instance, Solution sol) {
    cout << "--------------------------------" << endl;
    cout << "BEST IMPROVEMENT NEIGHBOURHOODS" << endl;

    cout << "transpose : " << endl;
    Solution st = getBestTransposeNeighbour(sol, instance, "FI");
    printSol(st);

    cout << "exchange : " << endl;
    Solution se = getBestExchangeNeighbour(sol, instance, "FI");
    printSol(se);

    cout << "insertion : " << endl;
    Solution si = getBestInsertionNeighbour(sol, instance, "FI");
    printSol(si);
}

/***************************************************************************/

string getInstanceName(int i, int j) {
    // i = represent wheter it is a instance with 50 or 100 jobs
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

void writeAllInstancesResToFile(vvint all_wcts, vvdouble all_rpds, string header, vector<string> filenames) {
    vector<vector<string>> modes = {vector<string>{"R", "SRZ"}, vector<string>{"FI", "BI"}, vector<string>{"T", "E", "I"}};


//	vector<string> filenames = {"results/all_algos_all_instances_wct.txt", "results/all_algos_all_instances_rpd.txt"};
								
    int nb_algos = all_wcts[0].size();
    for (int k = 0; k < 2; k++) {
        ofstream myfile;
        myfile.open(filenames[k]);
        myfile << header;

        string line;
		int global_index = 0;
		// TODO : change to 2
		for (int inst_nb_index = 0; inst_nb_index < 2; inst_nb_index++) { // 0 = instances 50, 1 = instances 100
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
    myfile << header;
    string line;
    // TODO : change to 2
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



double relativePercentageDeviation(int wct, int best_known) {
    /*
    Computes the relative percentage deviation of the algorithm k on the instance i
    algorithm k = a combination of different mode
    for example : --random --firstImprovement --tranpose
    */
    return 100 * (((double) wct - (double) best_known) / (double) best_known);
}

void testAllAlgosII(PfspInstance instance, vdouble &computation_times, vdouble &RPD, int best_known, vvint &all_wcts, int inst_nb) {
    /*
    1)  R - FI - T          7)  S - FI - T
    2)  R - FI - E          8)  S - FI - E
    3)  R - FI - I          9)  S - FI - I
    4)  R - BI - T          10) S - BI - T
    5)  R - BI - E          11) S - BI - E
    6)  R - BI - I          12) S - BI - I
    */
    srand ( time(NULL) ); // RANDOM SEED per instance
    cout << "=== TEST ALL ALGOS (12) ===" << endl;
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
    cout << "====================================" << endl;
}



/***************************************************************************/

void IItestAllInstances(vvint & all_wcts, vvdouble & all_rpds, vvdouble & avg_CTs, vvdouble & avg_RPDs, vvint best_knowns) {
    /*
    Run the 12 algos on each instance
     => 12 algos on 30 instances of 50 jobs and then on 30 instances of 100 jobs
     result = an average relative percentage deviation per algo + average computation time of each algo
     and one average per job nb so 50 and 100 => 2 averages per algo => 24 in total (12 algos)
    */

    // string base_filename = FILE_INSTANCES;
    string base_filename = UFILE_INSTANCES;
    string instance_name;

    int inst_nb = 0;

    auto start = chrono::steady_clock::now();
    // all instances with 50 and then 100 jobs :

    for (int i = 0; i < 2; i++) {  // TODO mettre i < 2 pour avoir les instances 100
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

    // TODO : diviser les sommmes par le nombre d'élements pour avoir l'avg
    cout << "END ========================= " << endl;
    cout << "All instances done in " << timing << "sec" << endl;
    cout << "average CT 50 jobs : " << avg_CTs[0] << endl;
    cout << "average CT 100 jobs : " << avg_CTs[1] << endl;
    cout << "average RPD 50 jobs : " << avg_RPDs[0] << endl;
    cout << "average RPD 100 jobs : " << avg_RPDs[1] << endl;

    for (int k = 0; k < all_wcts.size(); k++) {
        cout << "instance " << k << endl;
        cout << all_wcts[k] << endl;
    }
}

/***************************************************************************/



void testAllAlgosVND(PfspInstance instance, vdouble &computation_times, vdouble &RPD, vvint &all_wcts, int best_known, int inst_nb) {
    /*
     Test all combination of VND-PFSP on 1 instance
     result = a relative percentage deviation per algo + computation time per algo
     on the given instance.

     1) R-T-E-I   2) R-T-I-E   3) S-T-E-I   4) S-T-I-E
    */
    cout << "=== TEST ALL VND (4) ===" << endl;
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
            cout << "ALGO " << modes[0][s] << "-" << modes[n][0] << "-" <<
                 modes[n][1] << "-" << modes[n][2] << endl;
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

    for (int i = 0; i < 2; i++) {  // TODO mettre i < 2 pour avoir les instances 100
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

    // TODO : diviser les sommmes par le nombre d'élements pour avoir l'avg
    cout << "END ========================= " << endl;
    cout << "All instances done in " << timing << "sec" << endl;
    cout << "average CT 50 jobs : " << avg_CTs[0] << endl;
    cout << "average CT 100 jobs : " << avg_CTs[1] << endl;
    cout << "average RPD 50 jobs : " << avg_RPDs[0] << endl;
    cout << "average RPD 100 jobs : " << avg_RPDs[1] << endl;
    for (int k = 0; k < all_wcts.size(); k++) {
        cout << "instance " << k << endl;
        cout << all_wcts[k] << endl;
    }
}

/***************************************************************************/

void testSmallInstance() {
    cout << "========= TEST small instance =================================" << endl;
    PfspInstance small_instance;

    // string filenameSmall = FILE_INST_SMALL;
    string filenameSmall = UFILE_INST_SMALL;

    if (! small_instance.readDataFromFile(filenameSmall) )
        return;

    vector<int> sol = {0,1,2,3,4};

    /* Compute the WCT of this solution */
    int wct = small_instance.computeWCT(sol);
    cout << "WCT: " << wct << endl;

    Solution srz_sol = simplifiedRZheuristic(small_instance);
    printSol(srz_sol);

//    Solution s = {sol, wct};
//    testNeighbourBestImprovement(small_instance, s);

    vdouble computation_times(12);
    vdouble RPD(12);
    vvint all_wcts(60);
    testAllAlgosII(small_instance, computation_times, RPD, 144, all_wcts, 0);
    cout << "==================================" << endl;

    testAllAlgosVND(small_instance, computation_times, RPD, all_wcts, 144, 0);
}

void testMediumInstance(vvint best_knowns) {
    cout << "========= TEST instance MEDIUM (50) ================" << endl;

    // string filename = FILE_INST_MEDIUM;
    string filename = UFILE_INST_MEDIUM;

    /* Create instance object */
    PfspInstance instance;

    /* Read data from file */
//    if (! instance.readDataFromFile(argv[1]) )
    if (! instance.readDataFromFile(filename) ) return;

    /* solution with initial random values */
    Solution random_solution = generateRndSol(instance);

    cout << "Random solution: " << endl;
//    printSol(random_solution);

    /* Compute the WCT of this solution */
    int wct = instance.computeWCT(random_solution.sol);
    cout << "WCT of random sol : " << wct << endl;

    vdouble computation_times(12);
    vdouble RPD(12);
    vvint all_wcts(60);
    testAllAlgosII(instance, computation_times, RPD, best_knowns[0][0], all_wcts, 0);

    cout << "computation times : " << computation_times << endl;
    cout << "relative percentage deviations : " << RPD << endl;
    cout << "==================================" << endl;
//    testVNDall(instance);
}

void testBigInstance(vvint best_knowns) {
    cout << "=============== TEST instance BIG (100) ================" << endl;
    // string filename = FILE_INST_BIG;
    string filename = UFILE_INST_BIG;

    /* Create instance object */
    PfspInstance instance;

    /* Read data from file */
//    if (! instance.readDataFromFile(argv[1]) )
    if (! instance.readDataFromFile(filename) ) return;

    /* solution with initial random values */
    Solution random_solution = generateRndSol(instance);

    cout << "Random solution: " << endl;
    printSol(random_solution);

    /* Compute the WCT of this solution */
    int wct = instance.computeWCT(random_solution.sol);
    cout << "WCT of random sol : " << wct << endl;

    vdouble computation_times(12);
    vdouble RPD(12);
    vvint all_wcts(60);
    testAllAlgosII(instance, computation_times, RPD, best_knowns[1][0], all_wcts, 0);
    cout << "==================================" << endl;
//    testVNDall(instance);
}



/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

int main(int argc, char *argv[])
{

//    if (argc == 1) {
//        cout << "Usage: ./flowshopWCT <instance_file>" << endl;
//        return 0;
//    }

    /*
    # ARGUMENTS :
    # neighbourhood : --transpose, -- exchange, --insertion
    # initial solution : --random, --srz
    # pivoting rule : --bestImprovement, --firstImprovement
     */

    /* initialize random seed: */
    srand ( time(NULL) );
//     srand(0);

    //  string filenameBestSols = FILE_BEST_KNOWN_SOLS;
     string filenameBestSols = UFILE_BEST_KNOWN_SOLS;
    vvint best_knowns =  readBestSolFromFile(filenameBestSols);
//    cout << best_knowns << endl;

//    testSmallInstance();
//    testMediumInstance(best_knowns);
//    testBigInstance(best_knowns);

    // ======================================================
    // TEST ITERATIVE IMPROVEMENT ON ALL INSTANCES (50 + 100)
    vvint all_wcts(60); // 60 = nb of instances. Each sublist = 12 wct (12 algos)
    vvdouble all_rpds(60); // 60 = nb of instances. Each sublist = 12 wct (12 algos)
    vvdouble avg_CTs = {vdouble(12), vdouble(12)};
    vvdouble avg_RPDs = {vdouble(12), vdouble(12)};
    IItestAllInstances(all_wcts, all_rpds, avg_CTs, avg_RPDs, best_knowns);
    string header = "instance,R_FI_T,R_FI_E,R_FI_I,R_BI_T,R_BI_E,R_BI_I,S_FI_T,S_FI_E,S_FI_I,S_BI_T,S_BI_E,S_BI_I\n";
    // vector<string> filenames = {FILE_II_ALL_INST_WCT, FILE_II_ALL_INST_RPD};
    vector<string> filenames = {UFILE_II_ALL_INST_WCT, UFILE_II_ALL_INST_RPD};
    writeAllInstancesResToFile(all_wcts, all_rpds, header, filenames);
    // vector<string> filenamesII_avg = {FILE_II_AVG_RPDS, FILE_II_AVG_CTS};
    vector<string> filenamesII_avg = {UFILE_II_AVG_RPDS, UFILE_II_AVG_CTS};
    writeAlgosStatsToFile(filenamesII_avg[0], avg_RPDs, header);
    writeAlgosStatsToFile(filenamesII_avg[1], avg_CTs, header);

    // ======================================================
    // TEST VARIABLE NEIGHBOURHOOD DESCENT ON ALL INSTANCES (50 + 100)
    vvint all_wcts_VND(60); // 60 = nb of instances. Each sublist = 4 wct (4 algos)
    vvdouble all_rpds_VND(60); // 60 = nb of instances. Each sublist = 4 wct (4 algos)
    vvdouble avg_CTs_VND = {vdouble(4), vdouble(4)};
    vvdouble avg_RPDs_VND = {vdouble(4), vdouble(4)};
    VNDtestAllInstances(all_wcts_VND, all_rpds_VND, avg_CTs_VND, avg_RPDs_VND, best_knowns);
    string header_vnd = "instance,R-T-E-I,R-T-I-E,S-T-E-I,S-T-I-E\n";
    // vector<string> filenamesVND = {FILE_VND_ALL_INST_WCT, FILE_VND_ALL_INST_RPD};
    vector<string> filenamesVND = {UFILE_VND_ALL_INST_WCT, UFILE_VND_ALL_INST_RPD};
    writeAllInstancesResToFile(all_wcts_VND, all_rpds_VND, header_vnd, filenamesVND);
    // vector<string> filenamesVND_avg = {FILE_VND_AVG_RPDS, FILE_VND_AVG_CTS};
    vector<string> filenamesVND_avg = {UFILE_VND_AVG_RPDS, UFILE_VND_AVG_CTS};
    writeAlgosStatsToFile(filenamesVND_avg[0], avg_RPDs_VND, header_vnd);
    writeAlgosStatsToFile(filenamesVND_avg[1], avg_CTs_VND, header_vnd);

    return 0;
}