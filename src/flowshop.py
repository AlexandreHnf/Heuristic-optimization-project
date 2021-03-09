import pfsinstance
import random
import sys 
import time 
from operator import itemgetter
import copy

def generateRndPosition(mini, maxi):
    """ 
    Return a random number in the range [mini, maxi]
    """
    return random.randint(mini, maxi)

def randomPermutation(nb_jobs, sol):
    """
    Fill the solution with numbers between 0 and nb_jobs, shuffled
    """ 
    already_taken = [False for _ in range(nb_jobs)]
    choosen_nb = [0 for _ in range(nb_jobs)]

    nbj = -1
    for i in range(nb_jobs-1, -1, -1):
        rnd = generateRndPosition(0, i)
        nb_false = 0

        # find the rndth cell with value = false 
        j = 0
        while (nb_false < rnd):
            if (not already_taken[j]):
                nb_false += 1
            j += 1

        sol[j] = i 

        nbj += 1 
        choosen_nb[nbj] = j

        already_taken[j] = True 

def generateRndSol(nb_jobs):
    """
    Fill the solution with numbers between 0 and nb_jobs, shuffled
    """ 
    sol = [i for i in range(nb_jobs)]
    random.shuffle(sol)
    return sol

def getBestSubset(size, job, start, instance):
    # print("subset size : ", size)
    subset = start
    best_subset = []
    best_wct = 100000
    for i in range(size):
        subset.insert(i, job)

        temp_instance = pfsinstance.PfspInstance(len(subset), instance.getNbMac())
        temp_instance.setProcessingTimes(instance.getProcessingTimes())
        temp_instance.setWeights(instance.getWeights())
        wct = temp_instance.computeWCT(subset)

        print(">", subset, wct)
        if (wct <= best_wct):
            best_subset = copy.copy(subset) 
            best_wct = wct

        # print("subset before reset : ", subset)
        #reset the subset
        if (i < len(subset)):
            subset.pop(i)
        # subset = start
        # print("subset after reset : ", subset)

    return best_subset, best_wct



def simplifiedRZheuristic(instance):
    """
    Start by ordering the jobs in ascending order of their weighted sum of processing times
    Then, construct the solution by inserting one job at a time in the position that minimize the WCT
    """
    weighted_sums = {i: instance.weightedSumSingleJob(i) for i in range(instance.getNbJob())}
    starting_sequence= sorted(weighted_sums.items(), key=lambda x:x[1])
    
    print("weighted sums: ", weighted_sums)
    print("starting sequence : ", starting_sequence)

    best_sol = {"jobs":[], "wct": 100000000000} # step 1
    for j in range(len(starting_sequence)): # (job, T)
        print("Step {0} = {1} with J{2}".format(j, best_sol, starting_sequence[j][0]))

        bests = getBestSubset(j+1, starting_sequence[j][0], best_sol["jobs"], instance)
        best_sol["jobs"] = bests[0]
        best_sol["wct"] = bests[1]
        # print("best : ", best_sol)

    return best_sol

def generateInitialSolution(mode, instance):
    if (mode == "random"):
        return generateRndSol(instance.getNbJob())
    else:
        return simplifiedRZheuristic(instance)

def swapValues(temp_sol, i, j):
    # swap elements at position i and i + 1
    temp_val = temp_sol[i]
    temp_sol[i] = temp_sol[j]
    temp_sol[j] = temp_val 

def transposeNeighbourhood(sol, instance):
    """
    Two permutations ø, ø' are transpose neighbours if, and only if, 
    one can be obtained from the other by swapping the positions of two adjacent jobs
    ex : [A,B,C,D,E,F]
    =>   [A,C,B,D,E,F]
    """
    for i in range(0, len(sol)-1):
        temp_sol = copy.copy(sol)
        swapValues(temp_sol, i, i+1)
        print(temp_sol)

def exchangeNeighbourhood(sol, instance):
    """
    Two permutations ø, ø' are 2-exchange neighbours if, and only if, 
    one can be obtained from the other by exchanging two jobs at arbitrary positions
    ex : [A,B,C,D,E,F]
    =>   [A,E,C,D,B,F]
    """
    for i in range(len(sol)):
        for j in range(len(sol)):
            if (i != j):
                temp_sol = copy.copy(sol)
                swapValues(temp_sol, i, j)
                print(temp_sol)

def insertionNeighbourhood(sol, instance):
    """
    Two permutations ø, ø' are insertion neighbours if, and only if, 
    one can be obtained from the other by removing a job from one position 
    and inserting it at another position
    ex : [A,B,C,D,E,F]
    =>   [A,C,D,B,E,F]

    QUESTION : l'insertion peuut se faire en fin de liste aussi ? 
    """
    for i in range(len(sol)):
        for j in range(len(sol)+1):
            if (i != j and j != i+1): #to avoid useless solutions
                temp_sol = copy.copy(sol)
                temp_sol.insert(j, sol[i])
                if (j < i):
                    temp_sol.pop(i+1)
                else:
                    temp_sol.pop(i)
                print(temp_sol)

def bestImprovement(sol, instance):
    """
    Choose best from all neighbours of sol
    """
     

def firstImprovement(sol, instance):
    """
    Choose first improving neighbour (evaluate neighbours in fixed order)
    """
    pass     

def isLocalOptimal(sol):
    pass 

def chooseNeighbour():
    pass 


def iterativeImprovement():
    """
    Heuristic algorithm to find the optimal solution to the PSFP
    """
    pass
    # solution = generateInitialSolution()
    # while not isLocalOptimal(solution):
    #     neighbour = chooseNeighbour()
    #     solution = neighbour



def main():
    
    # ==========================================================
    # ================= TEST WCT ======================
    # ==========================================================
    # if (len(sys.argv) == 1):
    #     print("Usage : fllowshopWCT <instance_file> ")
    #     return

    # ARGUMENTS :
    # neighbourhood : --transpose, -- exchange, --insertion
    # initial solution : --random, --srz
    # pivoting rule : --bestImprovement, --firstImprovement

    # Initialize random seed 
    random.seed(time.time())

    #create instance object 
    instance = pfsinstance.PfspInstance()

    # read data from file
    filename = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\instances\\50_20_01"
    # if (not instance.readDataFromFile(argv[1])):
    if (not instance.readDataFromFile(filename)):
        return 

    # fill the vector with a random permutation
    solution = generateRndSol(instance.getNbJob())

    print("random solution ({0}): {1}".format(len(solution), solution))

    # Compute the WCT of this solution
    WCT = instance.computeWCT(solution)
    print("Total weighted completion time : ", WCT)

    # ==========================================================
    # ==================== TEST example ========================
    # ==========================================================

    print("TEST WCT")
    p_test = pfsinstance.PfspInstance(5, 3) # 5 jobs, 3 machines
    # p_test.allowMatrixMemory()
    p_test.processing_times = [[3,2,4], [3,1,2], [4,3,1], [2,3,2], [3,1,3]]
    w = [1,2,4,2,3]
    for i in range(5):
        p_test.setWeight(i, w[i])
    
    sol = [0,1,2,3,4]
    print("WCT : ", p_test.computeWCT(sol))
    p_test.showStats()

    test_sol = simplifiedRZheuristic(p_test)
    print("Initial Solution found : ", test_sol)

    # test neighbourhood :

    print("transpose: ")
    transposeNeighbourhood(["A", "B", "C", "D", "E", "F"], instance)

    print("exchange: ")
    exchangeNeighbourhood(["A", "B", "C", "D", "E", "F"], instance)

    print("insertion: ")
    insertionNeighbourhood(["A", "B", "C", "D", "E", "F"], instance)
    
if __name__ == "__main__":
    main()