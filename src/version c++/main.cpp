#include "experimentsAssignment1.h"
#include "experimentsAssignment2.h"
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

void testRandom() {
    for (int i = 0; i < 10; i++) {
        float r = ((double) rand() / (RAND_MAX));
        cout << r << endl;
    }
}

void processArgv(int argc, char *argv[]) {
    if (argc == 1) {
        cout << "Usage: ./flowshopRUN <II> <init> (R, SRZ) <pivoting> (FI, BI) <neighbour> (T, E, I) <instance_file>" << endl;
        cout << "Usage: ./flowshopRUN <VND> <init> (R, SRZ) <n1> <n2> <n3> (T E I or T I E) <instance_file>" << endl;
        cout << "Usage: ./flowshopRUN <runAllExperimentsA1>" << endl;
        cout << "Usage: ./flowshopRUN <TS> <tenure> <max_time> <instance_file>" << endl;
        cout << "Usage: ./flowshopRUN <GA> <Pm> <Pc> <Pe> <COUNT> <pop size> <max_time> <instance_file>" << endl;
        cout << "Usage: ./flowshopRUN <runAllExperimentsA2>" << endl;
        cout << "Usage: ./flowshopRUN <runRTDexperiments>" << endl;
    } else {
        cout << argv[1] << endl;
        if (string(argv[1]) == "II") {
            cout << argv[5] << argv[2] << argv[3] << argv[4] << endl;
            testInstanceII(string(argv[5]), string(argv[2]), string(argv[3]), string(argv[4]));
        } else if (string(argv[1]) == "VND") {
            vector<string> neighbour_rules = {string(argv[3]), string(argv[4]), string(argv[5])};
            testInstanceVND(string(argv[6]), string(argv[2]), neighbour_rules);
        } else if (string(argv[1]) == "runAllExperimentsA1"){
            string filenameBestSols = UFILE_BEST_KNOWN_SOLS;
            vvint best_knowns =  readBestSolFromFile(filenameBestSols);
            runAllExperimentsA1(best_knowns);
        } else if (string(argv[1]) == "TS") {
            testInstanceTabu(stoi(argv[2]), stof(argv[3]), string(argv[4]));
        } else if (string(argv[1]) == "GA") {
            testInstanceMGA(stof(argv[2]), stof(argv[3]), stof(argv[4]), stoi(argv[5]),
                            stoi(argv[6]), stof(argv[7]), string(argv[8]));
        } else if (string(argv[1]) == "runAllExperimentsA2") {
            string filenameBestSols = UFILE_BEST_KNOWN_SOLS;
            vvint best_knowns =  readBestSolFromFile(filenameBestSols);
            runAllExperimentsA2(best_knowns);
        } else if (string(argv[1]) == "runRTDexperiments") {
            string filenameBestSols = UFILE_BEST_KNOWN_SOLS;
            vvint best_knowns =  readBestSolFromFile(filenameBestSols);
            runAllRTDexperiments(best_knowns);
        }
    }
}

int main(int argc, char *argv[]) {

    // =====================================================
    // ===================: ASSIGNMENT 1 :==================
    // =====================================================
    /*
    # ARGUMENTS :
    # neighbourhood : --transpose (T), -- exchange (E), --insertion (I)
    # initial solution : --random (R), --srz (SRZ)
    # pivoting rule : --bestImprovement (BI), --firstImprovement (FI)
     */

    processArgv(argc, argv);

    // =====================================================
    // ===================: ASSIGNMENT 2 :==================
    // =====================================================
    /*
    # ARGUMENTS :
    # Tbu : tabu tenure, max time
    # GA : Prob mutation, Prob crossover, Prob elite, max count, pop size, max time
    */

//     testRandom();
//    testMGA();
//    testTabu();

    // =====================================================
    return 0;
}

