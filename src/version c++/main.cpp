#include <iostream>
#include <cstdlib>
#include <ctime>
#include "pfspinstance.h"
#include "flowshop.h"

using namespace std;

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
}

void testInstance1() {
    cout << "========= TEST instance 1 ======================================" << endl;

    string bestSolutionsFileW = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\instances\\bestSolutions.txt";
    string filenameW = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\instances\\50_20_01";
    char *filenameW2 = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\instances\\50_20_01";

    /* Create instance object */
    PfspInstance instance;

    /* Read data from file */
//    if (! instance.readDataFromFile(argv[1]) )
    if (! instance.readDataFromFile(filenameW2) ) return;

    /* solution with initial random values */
    vector<int> solution = generateRndSol(instance.getNbJob());

    cout << "Random solution: " << instance.getNbJob() << " => ";
    for (int i = 0; i < instance.getNbJob(); ++i)
        cout << solution[i] << " " ;
    cout << endl;

    /* Compute the WCT of this solution */
    int wct = instance.computeWCT(solution);
    cout << "WCT: " << wct << endl;
}

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

    testInstance1();

    testSmallInstance();


    return 0;
}