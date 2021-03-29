#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
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

double relativePercentageDeviation(int wct, int best_known) {
    /*
    Computes the relative percentage deviation of the algorithm k on the instance i
    algorithm k = a combination of different mode
    for example : --random --firstImprovement --tranpose
    */
    return 100 * (((double) wct - (double) best_known) / (double) best_known);
}

void testAllAlgos(PfspInstance instance, vdouble &computation_times, vdouble &RPD, int best_known) {
    /*
    1)  R - FI - T          7)  S - FI - T
    2)  R - FI - E          8)  S - FI - E
    3)  R - FI - I          9)  S - FI - I
    4)  R - BI - T          10) S - BI - T
    5)  R - BI - E          11) S - BI - E
    6)  R - BI - I          12) S - BI - I
    */

    cout << "=== TEST ALL ALGOS (12) ===" << endl;
    vector<string> init_rules = {"R", "SRZ"};
    vector<string> pivoting_rules = {"FI", "BI"};
    vector<string> neigh_modes = {"T", "E", "I"};
    int algo_id = 0;
    Solution start_sol_R = generateInitialSolution(init_rules[0], instance);
    Solution start_sol_SRZ = generateInitialSolution(init_rules[1], instance);
    vector<Solution> init_sols = {start_sol_R, start_sol_SRZ};
    cout << "---------------------------" << endl;

    auto start = chrono::steady_clock::now();
    Solution solution;
    for (int s = 0; s < 2; s++) {
        for (int p = 0; p < 2; p++) {
            for (int n = 0; n < 3; n++) {
                auto start_alg_time = chrono::steady_clock::now();
                solution = iterativeImprovement(pivoting_rules[p], neigh_modes[n], instance, init_sols[s]);
                cout << "ALGO " << init_rules[s] << "-" << pivoting_rules[p] << "-" << neigh_modes[n] << ": " << endl;
                printSol(solution);
                auto end_alg_time = chrono::steady_clock::now();
                computation_times[algo_id] = chrono::duration <double> (end_alg_time-start_alg_time).count();
                RPD[algo_id] = relativePercentageDeviation(solution.wct, best_known);
                cout << "==> " << chrono::duration <double> (end_alg_time-start_alg_time).count() << "sec" << endl;
                cout << "-------------------" << endl;
                algo_id++;
            }
        }
    }
    auto end = chrono::steady_clock::now();
    cout << "Done in " << chrono::duration <double> (end-start).count() << "sec" << endl;
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

void testAllInstances() {

}

/***************************************************************************/

void testSmallInstance() {
    cout << "========= TEST small instance =================================" << endl;
    PfspInstance small_instance;

    string filenameSmall = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\small instance\\05_03_01";
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
    testAllAlgos(small_instance, computation_times, RPD, 144);
    cout << "==================================" << endl;

    testVNDall(small_instance, computation_times, RPD, 144);
}

void testMediumInstance(vector<vector<int>> best_knowns) {
    cout << "========= TEST instance MEDIUM (50) ================" << endl;

    string bestSolutionsFileW = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\instances\\bestSolutions.txt";
    string filenameW = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\instances\\50_20_01";

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
    testAllAlgos(instance, computation_times, RPD, best_knowns[1][0]);

    cout << "computation times : " << computation_times << endl;
    cout << "relative percentage deviations : " << RPD << endl;
    cout << "==================================" << endl;
//    testVNDall(instance);
}

void testBigInstance(vector<vector<int>> best_knowns) {
    cout << "=============== TEST instance BIG (100) ================" << endl;
//    char *filenameW = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\instances\\100_20_01";
    string filenameW = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\instances\\100_20_01";

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
    testAllAlgos(instance, computation_times, RPD, best_knowns[0][0]);
    cout << "==================================" << endl;
//    testVNDall(instance);
}



/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

int main(int argc, char *argv[])
{
    int i;
    long int WeightedSumCompletionTimes;

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
    // srand(0);

//    char *filenameBestSols = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\instances\\bestSolutions.txt";
    string filenameBestSols = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\instances\\bestSolutions.txt";
    vector<vector<int>> best_knowns =  readBestSolFromFile(filenameBestSols);

//    testSmallInstance();

    testMediumInstance(best_knowns);

//    testBigInstance(best_knowns);


    return 0;
}