//
// Created by Alexandre HENEFFE on 0026 26 mars 2021.
//

#ifndef _PFSPINSTANCEWT_H_
#define _PFSPINSTANCEWT_H_

#include <string>
#include <vector>


class PfspInstance{
    private:
        int nb_jobs;
        int nb_mach;
        std::vector< long int > dueDates;
        std::vector< long int > weights;

        std::vector< std::vector <long int> > processing_times;
        std::vector < int > best_closest_solutions;

    public:
        PfspInstance();
        ~PfspInstance();

        /* Read write privates attributs : */
        int getNbJob();
        void setNbJob(int nJ);
        int getNbMac();
        void setNbMac(int nM);

        /* Allow the memory for the processing times matrix : */
        void allowMatrixMemory(int nbJ, int nbM);

        /* Read\Write values in the matrix : */
        long int getTime(int job, int machine);
        void setTime(int job, int machine, long int processTime);

        long int getDueDate(int job);
        void setDueDate(int job, int value);

        long int getPriority(int job);
        void setPriority(int job, int value);

        void setBestSolutions(int best_known);
        int getNbBestSols();
        int getClosestSolution(int i);

        /* Read Data from a file : */
        bool readDataFromFile(std::basic_string<char> fileName);


        void WCTfirstMach(std::vector<long int> & prev_mach_endtimes, std::vector< int > & sol);
        void WCTotherMath(std::vector<long int> & prev_mach_endtimes, long int &prev_job_endtime, std::vector< int > & sol);
        long int computeWCT (std::vector< int > & sol);

        double weightedSumSingleJob(int job);
};

#endif

