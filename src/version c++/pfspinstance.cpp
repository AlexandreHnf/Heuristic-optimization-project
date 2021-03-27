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
#include <fstream>
#include <cmath>
#include <cstring>
#include "pfspinstance.h"


using namespace std;

PfspInstance::PfspInstance() {}

PfspInstance::~PfspInstance() {}

int PfspInstance::getNbJob() {
    return nb_jobs;
}

int PfspInstance::getNbMac() {
    return nb_mach;
}



/* Allow the memory for the processing times matrix : */
void PfspInstance::allowMatrixMemory(int nbJ, int nbM)
{
    processing_times.resize(nbJ);          // 1st dimension

    for (int cpt = 0; cpt < nbJ; ++cpt)
        processing_times[cpt].resize(nbM); // 2nd dimension

    dueDates.resize(nbJ);
    weights.resize(nbJ);
}


long int PfspInstance::getTime(int job, int machine)
{
    if ((job < 0) || (job >= nb_jobs) || (machine < 0) || (machine >= nb_mach))
        cout    << "ERROR. file:pfspInstance.cpp, method:getTime. Out of bound. job=" << job
                << ", machine=" << machine << std::endl;

    return processing_times[job][machine];
}


/* Read the instance from file : */
bool PfspInstance::readDataFromFile(char* fileName) {
    // TODO : must be working on ubuntu and windows with relative path

    bool everythingOK = true;
    int j, m; // iterators
    long int readValue;
    string str;
    ifstream fileIn;
    char * aux2;
    char fileNameOK[100] = "";

    aux2 = (strrchr(fileName, '\\')); // find last occurence of '/' in filename

    if (aux2 == NULL)
        aux2 = fileName;
    else
        aux2 += 1;

    strcat(fileNameOK, aux2);

    cout << "name : " << fileNameOK << endl;
    cout << "file : " << fileName << endl;

    fileIn.open(fileName);

    if ( fileIn.is_open() ) {
        fileIn >> nb_jobs;
        fileIn >> nb_mach;
        cout << "Nb jobs : " << nb_jobs << ", nb mach : " << nb_mach << endl;
        allowMatrixMemory(nb_jobs, nb_mach);

        for (j = 0; j < nb_jobs; ++j) {
            for (m = 0; m < nb_mach; ++m) {
                fileIn >> readValue; // The number of each machine, not important !
                fileIn >> readValue; // Process Time
                processing_times[j][m] = readValue;
            }
        }
        fileIn >> str; // this is not read

        for (j = 0; j < nb_jobs; ++j) {
            fileIn >> readValue; // -1
            fileIn >> readValue;
            dueDates[j] = readValue;
            fileIn >> readValue; // -1
            fileIn >> readValue;
            weights[j] = readValue;
        }

        cout << "All is read from file." << std::endl;
        fileIn.close();
    }
    else {
        cout    << "ERROR. file:pfspInstance.cpp, method:readDataFromFile, "
                << "error while opening file " << fileName << endl;

        everythingOK = false;
    }

    return everythingOK;
}

void PfspInstance::WCTfirstMach(vector<long int> & prev_mach_endtimes, vector< int > & sol) {
    /* 1st machine : */
    prev_mach_endtimes[0] = 0;
    for ( int j = 0; j < sol.size(); ++j ) {
        int job_nb = sol[j];
        int prev = 0;
        if (j-1 > -1)
            prev = prev_mach_endtimes[j-1];
        prev_mach_endtimes[j] = prev + processing_times[job_nb][0];
    }
}

void PfspInstance::WCTotherMath(vector<long int> & prev_mach_endtimes, long int &prev_job_endtime, vector< int > & sol) {
    int job_nb;
    for (int m = 1; m < nb_mach; ++m ) {
        prev_mach_endtimes[0] += processing_times[sol[0]][m];
        prev_job_endtime = prev_mach_endtimes[0];

        for ( int j = 1; j < sol.size(); ++j ) {
            job_nb = sol[j];

            if ( prev_mach_endtimes[j] > prev_job_endtime ) {
                prev_mach_endtimes[j] = prev_mach_endtimes[j] + processing_times[job_nb][m];
                prev_job_endtime = prev_mach_endtimes[j];
            } else {
                prev_job_endtime += processing_times[job_nb][m];
                prev_mach_endtimes[j] = prev_job_endtime;
            }
        }
    }
}

/* Compute the weighted sum of completion time of a given solution */
long int PfspInstance::computeWCT(vector< int > & sol)
{
    int j, m;
    int job_nb;
    long int wct;

    /* We need end times on previous machine : */
    vector< long int > prev_mach_endtimes ( nb_jobs );
    /* And the end time of the previous job, on the same machine : */
    long int prev_job_endtime;

    /* 1st machine : */
    WCTfirstMach(prev_mach_endtimes, sol);

    /* others machines : */
    WCTotherMath(prev_mach_endtimes, prev_job_endtime, sol);

    wct = 0;
    for ( j = 0; j < sol.size(); ++j )
        wct += prev_mach_endtimes[j] * weights[sol[j]];

    return wct;
}

/* Computes the weighted sum of processing times of job Ji */
double PfspInstance::weightedSumSingleJob(int job) {
    double ws = 0;
    for (int j = 0; j < nb_mach; j++) {
        ws += processing_times[job][j] / (double) weights[job];
    }
    return ws;
}
