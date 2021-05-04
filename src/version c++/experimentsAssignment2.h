//
// Created by Alexandre on 0027 27 avril 2021.
//

#ifndef VERSION_C___EXPERIMENTSASSIGNMENT2_H
#define VERSION_C___EXPERIMENTSASSIGNMENT2_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include "pfspinstance.h"
#include "flowshop.h"
#include "MemeticGA.h"
#include "tabu.h"

void printPopulation(Population pop) {
    for (auto p : pop) {
        printSol(p);
    }
}

void testMutation() {
    Solution child = {{1,2,3,4,5,6,7,8,9}, 0};
    printSol(child);

    mutation(child, 0.2);
    printSol(child);
}

void testSorted(Population pop) {
    Population sorted = getSortedPopulation(pop);
    printPopulation(sorted);
}

void testEliteSelection(Population pop) {
    Population elite = eliteSelection(0.6, pop);
    printPopulation(elite);
}

void testGetAC(PfspInstance instance) {
    vvInt M = {
            {0,8,5,0,2},
            {8,2,4,0,1},
            {1,5,0,9,0},
            {4,0,2,6,3},
            {2,0,4,0,9}
    };
    Solution ac = getArtificialChromosome(instance, M, 5);
    printSol(ac);
}

void testWSMGS(PfspInstance instance, Population E) {
    Population sorted = getSortedPopulation(E);
    Solution ac = WSMGS(instance, sorted);
    printSol(ac);
}

void testCrossover(PfspInstance instance, Solution p1, Solution p2) {
    Solution ac = {{2,1,4,3,5}, 0};
    Solution son = crossover(instance, p1, p2, ac, 0.6);
    printSol(son);
    Solution daughter = crossover(instance, p2, p1, ac, 0.6);
    printSol(daughter);
}

void testRandom() {
    //    srand( (unsigned)time( NULL ) );
//    std::srand(std::time(nullptr));
    std::srand(std::time(nullptr));
    float r = ((double) rand() / (RAND_MAX));
    cout << r << endl;
//    cout << getRandom01() << endl;
    cout << getRandomIndex(0,10, -1) << endl;
}

void testMGASmallInstance(float Pe, float Pc, float Pm, int COUNT) {
    PfspInstance instance_small;

    string filename_small = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\05_03_01.txt";
    if (! instance_small.readDataFromFile(filename_small) )
        return;
    cout << "ok parse" << endl;
    Solution bs = memeticGeneticAlgo(instance_small, 5, Pe, Pc, Pm, COUNT);
    printSol(bs);
}

void testMGAmediumInstance(float Pe, float Pc, float Pm, int COUNT) {
    PfspInstance instance_medium;

    string filename_medium = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\instances\\50_20_01";
    if (! instance_medium.readDataFromFile(filename_medium) )
        return;
    cout << "ok parse" << endl;
    Solution bs = memeticGeneticAlgo(instance_medium, 5, Pe, Pc, Pm, COUNT);
    printSol(bs);
}

void testMGA() {
    PfspInstance instance;
    instance.setNbJob(5);
    instance.setNbMac(5);
    //testMutation();

    Solution p1 = {{2,3,1,4,5}, 15};
    Solution p2 = {{2,1,5,3,4}, 34};
    Solution p3 = {{3,1,5,4,2}, 46};
    Solution p4 = {{5,2,4,3,1}, 38};
    Solution p5 = {{4,1,2,3,5}, 24};
    Population pop = {p1, p2, p3, p4, p5};

//    testSorted(pop);
//    testEliteSelection(pop);
//    testGetAC(instance);
//    testWSMGS(instance, pop)
//    testCrossover(instance, p1, p3);

    float Pm = 0.5;
    float Pc = 1.0;
    float Pe = 0.6;
    int COUNT = 100;

//    testRandom();
//    testMGASmallInstance(Pe, Pc, Pm, COUNT);
    testMGAmediumInstance(Pe, Pc, Pm, COUNT);


}

// ==================================================================
// =================== TEST TABU ====================

void testTabuSmall(int tabu_tenure) {
    PfspInstance instance_small;

    string filename_small = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\05_03_01.txt";
    if (! instance_small.readDataFromFile(filename_small) )
        return;
    cout << "ok parse" << endl;
    Solution bs = tabuSearch(instance_small, tabu_tenure);
    printSol(bs);
}

void testTabuMedium(int tabu_tenure) {
    PfspInstance instance_medium;

    string filename_medium = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\src\\version c++\\instances\\50_20_01";
    if (! instance_medium.readDataFromFile(filename_medium) )
        return;
    cout << "ok parse" << endl;
    Solution bs = tabuSearch(instance_medium, tabu_tenure);
    printSol(bs);
}

void testTabuInsert() {
    vInt pi = {1,2,3,4,5};
    for (int a = 0; a < 5; a++) {
        for (int b = 0; b < 5; b++) {
            if (a != b) {
                vInt tmp = pi;
                if (a < b)
                    insertRight(a, b, tmp);
                if (a > b)
                    insertLeft(a, b, tmp);
                cout << a << "," << b << endl;
                printVector(tmp, "");
            }
        }
    }
}

void testTabu() {

//    testTabuInsert();
//    testTabuSmall(4);
    testTabuMedium(7);
}

#endif //VERSION_C___EXPERIMENTSASSIGNMENT2_H
