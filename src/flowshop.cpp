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
#include "pfspinstance.h"

using namespace std;


int generateRndPosition(int min, int max) {
    return ( rand() % max + min );
}


/* Fill the solution with numbers between 1 and nbJobs, shuffled */
void randomPermutation(int nbJobs, vector< int > & sol)
{
  vector<bool> alreadyTaken(nbJobs+1, false); // nbJobs elements with value false
  vector<int > choosenNumber(nbJobs+1, 0);

  int nbj;
  int rnd, i, j, nbFalse;

  nbj = 0;
  for (i = nbJobs; i >= 1; --i)
  {
    rnd = generateRndPosition(1, i);
    nbFalse = 0;

    /* find the rndth cell with value = false : */
    for (j = 1; nbFalse < rnd; ++j)
      if ( ! alreadyTaken[j] )
        ++nbFalse;
    --j;

    sol[j] = i;

    ++nbj;
    choosenNumber[nbj] = j;

    alreadyTaken[j] = true;
  }
}

/***********************************************************************/

int main(int argc, char *argv[])
{
  int i;
  long int WeightedSumCompletionTimes;


  if (argc == 1)
  {
    cout << "Usage: ./flowshopWCT <instance_file>" << endl;
    return 0;
  }

  /* initialize random seed: */
  srand ( time(NULL) );

  /* Create instance object */
  PfspInstance instance;

  /* Read data from file */
  if (! instance.readDataFromFile(argv[1]) )
    return 1;

  /* Create a vector of int to represent the solution
    WARNING: By convention, we store the jobs starting from index 1,
             thus the size nbJob + 1. */
  vector< int > solution ( instance.getNbJob()+ 1 );

  /* Fill the vector with a random permutation */
  randomPermutation(instance.getNbJob(), solution);

  cout << "Random solution: " ;
  for (i = 1; i <= instance.getNbJob(); ++i)
    cout << solution[i] << " " ;
  cout << endl;

  /* Compute the WCT of this solution */
  WeightedSumCompletionTimes = instance.computeWCT(solution);
  cout << "Total weighted completion time: " << WeightedSumCompletionTimes << endl;

  return 0;
}
