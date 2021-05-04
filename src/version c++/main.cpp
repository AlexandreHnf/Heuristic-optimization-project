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

int main(int argc, char *argv[]) {

    /*
    # ARGUMENTS :
    # neighbourhood : --transpose (T), -- exchange (E), --insertion (I)
    # initial solution : --random (R), --srz (SRZ)
    # pivoting rule : --bestImprovement (BI), --firstImprovement (FI)
     */

//    processArgv(argc, argv);

    // =====================================================

    srand ( time(NULL) );
//     testRandom();

    testMGA();

//    testTabu();



    // =====================================================
    return 0;
}

