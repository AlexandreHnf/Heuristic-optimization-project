#include "experimentsAssignment1.h"
#include "experimentsAssignment2.h"
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

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

    testMGA();

    // =====================================================
    return 0;
}

