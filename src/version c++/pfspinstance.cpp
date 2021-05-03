//
// Created by Alexandre HENEFFE on 0026 26 mars 2021.
//

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

void PfspInstance::setNbJob(int nJ) {
    nb_jobs = nJ;
}

int PfspInstance::getNbMac() {
    return nb_mach;
}

void PfspInstance::setNbMac(int nM) {
    nb_mach = nM;
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
bool PfspInstance::readDataFromFile(basic_string<char> fileName) {

    bool everythingOK = true;
    int j, m; // iterators
    long int readValue;
    string str;
    ifstream fileIn;

    fileIn.open(fileName);

    if ( fileIn.is_open() ) {
        fileIn >> nb_jobs;
        fileIn >> nb_mach;
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
