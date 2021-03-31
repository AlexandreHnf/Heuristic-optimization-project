#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include "pfspinstance.h"
#include "flowshop.h"

using namespace std;
typedef vector<double> vdouble;

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

void writeAlgoResToFile(vector<vector<int>> all_wcts, vector<vector<double>> all_rpds) {
    vector<vector<string>> modes = {vector<string>{"R", "SRZ"}, vector<string>{"FI", "BI"}, vector<string>{"T", "E", "I"}};
    // vector<string> filenames = {"D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\results\\all_algos_all_instances_wct.txt",
    //                             "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\results\\all_algos_all_instances_rpd.txt"};

	vector<string> filenames = {"results/all_algos_all_instances_wct.txt", "results/all_algos_all_instances_rpd.txt"};
								

    for (int k = 0; k < 2; k++) {
        ofstream myfile;
        myfile.open(filenames[k]);
        myfile << "instance,R_FI_T,R_FI_E,R_FI_I,R_BI_T,R_BI_E,R_BI_I,S_FI_T,S_FI_E,S_FI_I,S_BI_T,S_BI_E,S_BI_I\n";

        string line;
		int global_index = 0;
		for (int inst_nb_index = 0; inst_nb_index < 2; inst_nb_index++) { // 0 = instances 50, 1 = instances 100
			for (int i = 0; i < 30; i++) { // TODO : change to all_wcts.size()
				
				line = getInstanceName(inst_nb_index, i + 1) + ","; // +1 bc it starts at 01
				for (int a = 0; a < 12; a++) {
					if (k == 0) { // if first file : WCTs
						line += to_string(all_wcts[global_index][a]);
					} else { // if second file : RPDs
						line += to_string(all_rpds[global_index][a]);
					}

					if (a < 12 - 1) {
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

/***************************************************************************/



double relativePercentageDeviation(int wct, int best_known) {
    /*
    Computes the relative percentage deviation of the algorithm k on the instance i
    algorithm k = a combination of different mode
    for example : --random --firstImprovement --tranpose
    */
    return 100 * (((double) wct - (double) best_known) / (double) best_known);
}

void testAllAlgos(PfspInstance instance, vdouble &computation_times, vdouble &RPD, int best_known, vector<vector<int>> &all_wcts, int inst_nb) {
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

void testVNDall(PfspInstance instance, vdouble &computation_times, vdouble &RPD, int best_known) {
    /*
     Test all combination of VND-PFSP on all instances
     result = a relative percentage deviation per algo + computation time of each algo
     on the given instance.
    */
    cout << "=== TEST ALL VND (4) ===" << endl;
    vector<vector<string>> modes = {vector<string>{"R", "SRZ"}, vector<string>{"T", "E", "I"}, vector<string>{"T", "I", "E"}};

    Solution start_sol_R = generateInitialSolution(modes[0][0], instance);
    Solution start_sol_SRZ = generateInitialSolution(modes[0][1], instance);
    vector<Solution> init_sols = {start_sol_R, start_sol_SRZ};
    cout << "---------------------------" << endl;
    Solution solution;

    auto start = chrono::steady_clock::now();
    for (int s = 0; s < 2; s++) {
        for (int n = 1; n < 3; n++) {
            auto start_alg_time = chrono::steady_clock::now();
            cout << "ALGO " << modes[0][s] << "-" << modes[n][0] << "-" <<
                            modes[n][1] << "-" << modes[n][2] << endl;
            solution = variableNeighbourhoodDescent(modes[n], instance, init_sols[s]);
            printSol(solution);
            auto end_alg_time = chrono::steady_clock::now();
            computation_times[s*2+n-1] = chrono::duration <double> (end_alg_time-start_alg_time).count();
            RPD[s*2+n-1] = relativePercentageDeviation(solution.wct, best_known);
            cout << "==> " << chrono::duration <double> (end_alg_time-start_alg_time).count() << "sec" << endl;
        }
    }
    auto end = chrono::steady_clock::now();
    double timing = chrono::duration <double> (end-start).count();
    cout << "4 VND Done in " << timing << "sec" << endl;
}

/***************************************************************************/

vector<vector<int>> IItestAllInstances(vector<vector<int>> & all_wcts, vector<vector<double>> & all_rpds, vector<vector<int>> best_knowns) {
    /*
    Run the 12 algos on each instance
     => 12 algos on 30 instances of 50 jobs and then on 30 instances of 100 jobs
     TODO : srand ( time(NULL) ); for each instance
    */
    vector<vdouble> avg_CTs = {vdouble(12), vdouble(12)};
    vector<vdouble> avg_RPDs = {vdouble(12), vdouble(12)};
    // string base_filenameW = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\instances\\";
    string base_filenameW = "instances/";
    string instance_name;

    int inst_nb = 0;

    auto start = chrono::steady_clock::now();
    // all instances with 50 and then 100 jobs :
    // TODO mettre i < 2 pour avoir les instances 100
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 30; j++) {
            instance_name = getInstanceName(i, j + 1);
            string inst_filename = base_filenameW + instance_name;
            cout << "Instance " << instance_name << ": " << endl;
            PfspInstance instance;
            instance.readDataFromFile(inst_filename);

            vdouble computation_times(12);
            vdouble RPD(12);
            testAllAlgos(instance, computation_times, RPD, best_knowns[i][j], all_wcts, inst_nb);
            for (int k = 0; k < 12; k++) {
                avg_CTs[i][k] += computation_times[k] / 30;
                avg_RPDs[i][k] += RPD[k] / 30;
                all_rpds[inst_nb].push_back(RPD[k]);
//                cout << "Computing times : " << computation_times << endl;
//                cout << "Relative percentage deviations : " << RPD << endl;
            }
            inst_nb++;
        }
    }
    auto end = chrono::steady_clock::now();
    double timing = chrono::duration <double> (end-start).count();

    // TODO : diviser les sommmes par le nombre d'élements pour avoir l'avg
    cout << "END ========================= " << endl;
    cout << "All instances done in " << timing << "sec" << endl;
    cout << "average CT : " << avg_CTs << endl;
    cout << "average RPD : " << avg_RPDs << endl;
    for (int k = 0; k < all_wcts.size(); k++) {
        cout << "instance " << k << endl;
        cout << all_wcts[k] << endl;
    }

    return all_wcts;
}

/***************************************************************************/

void testSmallInstance() {
    cout << "========= TEST small instance =================================" << endl;
    PfspInstance small_instance;

    // string filenameSmall = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\small instance\\05_03_01";
    string filenameSmall = "small instance/05_03_01";
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
    vector<vector<int>> all_wcts(60);
    testAllAlgos(small_instance, computation_times, RPD, 144, all_wcts, 0);
    cout << "==================================" << endl;

    testVNDall(small_instance, computation_times, RPD, 144);
}

void testMediumInstance(vector<vector<int>> best_knowns) {
    cout << "========= TEST instance MEDIUM (50) ================" << endl;

    // string filenameW = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\instances\\50_20_01";
    string filenameW = "instances/50_20_01";

    /* Create instance object */
    PfspInstance instance;

    /* Read data from file */
//    if (! instance.readDataFromFile(argv[1]) )
    if (! instance.readDataFromFile(filenameW) ) return;

    /* solution with initial random values */
    Solution random_solution = generateRndSol(instance);

    cout << "Random solution: " << endl;
//    printSol(random_solution);

    /* Compute the WCT of this solution */
    int wct = instance.computeWCT(random_solution.sol);
    cout << "WCT of random sol : " << wct << endl;

    vdouble computation_times(12);
    vdouble RPD(12);
    vector<vector<int>> all_wcts(60);
    testAllAlgos(instance, computation_times, RPD, best_knowns[0][0], all_wcts, 0);

    cout << "computation times : " << computation_times << endl;
    cout << "relative percentage deviations : " << RPD << endl;
    cout << "==================================" << endl;
//    testVNDall(instance);
}

void testBigInstance(vector<vector<int>> best_knowns) {
    cout << "=============== TEST instance BIG (100) ================" << endl;
    // string filenameW = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\instances\\100_20_01";
    string filenameW = "instances/100_20_01";

    /* Create instance object */
    PfspInstance instance;

    /* Read data from file */
//    if (! instance.readDataFromFile(argv[1]) )
    if (! instance.readDataFromFile(filenameW) ) return;

    /* solution with initial random values */
    Solution random_solution = generateRndSol(instance);

    cout << "Random solution: " << endl;
    printSol(random_solution);

    /* Compute the WCT of this solution */
    int wct = instance.computeWCT(random_solution.sol);
    cout << "WCT of random sol : " << wct << endl;

    vdouble computation_times(12);
    vdouble RPD(12);
    vector<vector<int>> all_wcts(60);
    testAllAlgos(instance, computation_times, RPD, best_knowns[1][0], all_wcts, 0);
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
//    srand ( time(NULL) );
     srand(0);

    // string filenameBestSols = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\instances\\bestSolutions.txt";
    string filenameBestSols = "instances/bestSolutions.txt";
    vector<vector<int>> best_knowns =  readBestSolFromFile(filenameBestSols);
//    cout << best_knowns << endl;

//    testSmallInstance();
//    testMediumInstance(best_knowns);
//    testBigInstance(best_knowns);

    vector<vector<int>> all_wcts(60); // 60 = nb of instances. Each sublist = 12 wct (12 algos)
    vector<vector<double>> all_rpds(60); // 60 = nb of instances. Each sublist = 12 wct (12 algos)
    IItestAllInstances(all_wcts, all_rpds, best_knowns);
    writeAlgoResToFile(all_wcts, all_rpds);


    return 0;
}