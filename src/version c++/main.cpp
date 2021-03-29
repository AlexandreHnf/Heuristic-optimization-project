#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "pfspinstance.h"
#include "flowshop.h"

using namespace std;

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
void runAllmodes(PfspInstance instance, Solution start_sol, string init_rule, vector<string> piv_rules, vector<string> neigh_modes) {
    Solution solution;
    for (int p = 0; p < 2; p++) {
        for (int n = 0; n < 3; n++) {
            solution = iterativeImprovement(piv_rules[p], neigh_modes[n], instance, start_sol);
            cout << "ALGO " << init_rule << "-" << piv_rules[p] << "-" << neigh_modes[n] << endl;
            printSol(solution);
        }
    }
}

void testAllAlgos(PfspInstance instance) {
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
                solution = iterativeImprovement(pivoting_rules[p], neigh_modes[n], instance, init_sols[s]);
                cout << "ALGO " << init_rules[s] << "-" << pivoting_rules[p] << "-" << neigh_modes[n] << ": " << endl;
                printSol(solution);
                cout << "-------------------" << endl;
            }
        }
    }
    auto end = chrono::steady_clock::now();
    cout << "Done in " << chrono::duration <double> (end-start).count() << "sec" << endl;
}

void testVNDall(PfspInstance instance) {
    /*
     Test all combination of VND-PFSP on all instances
    */
    cout << "=== TEST ALL VND (4) ===" << endl;
    vector<string> init_rules = {"R", "SRZ"};
    vector<string> neigh_modes1 = {"T", "E", "I"};
    vector<string> neigh_modes2 = {"T", "I", "E"};
    vector<vector<string>> neigh_modes = {neigh_modes1, neigh_modes2};

    Solution start_sol_R = generateInitialSolution(init_rules[0], instance);
    Solution start_sol_SRZ = generateInitialSolution(init_rules[1], instance);
    vector<Solution> init_sols = {start_sol_R, start_sol_SRZ};
    cout << "---------------------------" << endl;
    Solution solution;

    auto start = chrono::steady_clock::now();
    for (int s = 0; s < 2; s++) {
        for (int n = 0; n < 2; n++) {
            cout << "ALGO " << init_rules[s] << "-" << neigh_modes[n][0] << "-" <<
                            neigh_modes[n][1] << "-" << neigh_modes[n][2] << endl;
            solution = variableNeighbourhoodDescent(neigh_modes[n], instance, init_sols[s]);
            printSol(solution);
        }
    }
    auto end = chrono::steady_clock::now();
    cout << "Done in " << chrono::duration <double> (end-start).count() << "sec" << endl;
}

void testAllInstances() {

}

/***************************************************************************/

void testSmallInstance() {
    cout << "========= TEST small instance =================================" << endl;
    PfspInstance small_instance;

    char * filenameSmall = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\small instance\\05_03_01";
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

    testAllAlgos(small_instance);
    cout << "==================================" << endl;
    testVNDall(small_instance);
}

void testMediumInstance() {
    cout << "========= TEST instance MEDIUM (50) ================" << endl;

    string bestSolutionsFileW = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\instances\\bestSolutions.txt";
    string filenameW = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\instances\\50_20_01";
    char *filenameW2 = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\instances\\50_20_01";

    /* Create instance object */
    PfspInstance instance;

    /* Read data from file */
//    if (! instance.readDataFromFile(argv[1]) )
    if (! instance.readDataFromFile(filenameW2) ) return;

    /* solution with initial random values */
    Solution random_solution = generateRndSol(instance);

    cout << "Random solution: " << endl;
    printSol(random_solution);

    /* Compute the WCT of this solution */
    int wct = instance.computeWCT(random_solution.sol);
    cout << "WCT of random sol : " << wct << endl;

//    testAllAlgos(instance);
    cout << "==================================" << endl;
    testVNDall(instance);
}

void testBigInstance() {
    cout << "=============== TEST instance BIG (100) ================" << endl;
    char *filenameW = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\instances\\100_20_01";

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

//    testAllAlgos(instance);
    cout << "==================================" << endl;
    testVNDall(instance);
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

    char *filenameBestSols = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\instances\\bestSolutions.txt";
    vector<vector<int>> best_solutions =  readBestSolFromFile(filenameBestSols);

//    testSmallInstance();

//    testMediumInstance();

//    testBigInstance();


    return 0;
}