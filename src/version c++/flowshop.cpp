//
// Created by Alexandre on 0026 26 mars 2021.
//

/***************************************************************************
 *   Copyright (C) 2012 by Jérémie Dubois-Lacoste   *
 *   jeremie.dl@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "pfspinstance.h"
#include "flowshop.h"

using namespace std;


int generateRndPosition(int min, int max) {
    return ( rand() % max + min );
}


/* Fill the solution with numbers between 1 and nbJobs, shuffled */
void randomPermutation(int nbJobs, vector< int > & sol)
{
    vector<bool> alreadyTaken(nbJobs, false); // nbJobs elements with value false
    vector<int > choosenNumber(nbJobs, 0);

    int nbj;
    int rnd, i, j, nbFalse;

    nbj = -1;
    for (i = nbJobs-1; i >= 0; --i)
    {
        rnd = generateRndPosition(0, i);
        nbFalse = 0;

        /* find the rndth cell with value = false : */
        for (j = 0; nbFalse < rnd; ++j)
            if ( ! alreadyTaken[j] )
                ++nbFalse;
        --j;

        sol[j] = i;

        ++nbj;
        choosenNumber[nbj] = j;

        alreadyTaken[j] = true;
    }
}

std::vector<int> generateRndSol(int nbJ) {
    std::vector<int> sol(nbJ);
    for (int i=0; i<nbJ; i++) {
        sol[i] = i;
    }
    random_shuffle(sol.begin(), sol.end());
    return sol;
}

/***********************************************************************/
