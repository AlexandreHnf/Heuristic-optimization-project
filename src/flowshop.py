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

def randomPermutation(nb_jobs):
	"""
	Fill the solution with numbers between 0 and nb_jobs, shuffled
	""" 
	# TODO : fix the generate random solution function (to have an uniform random generation)
	already_taken = [False for _ in range(nb_jobs)]
	choosen_nb = [0 for _ in range(nb_jobs)]
	sol = [-1 for _ in range(nb_jobs)]

	nbj = -1
	for i in range(nb_jobs-1, -1, -1):
		rnd = generateRndPosition(0, i)
		# print("rnd: ", rnd)
		nb_false = 0

		# find the rndth cell with value = false 
		j = 0
		while (nb_false < rnd):
			if (not already_taken[j]):
				nb_false += 1
			j+=1
		j-= 1
		# print(already_taken, j)

		sol[j] = i 

		nbj += 1 
		choosen_nb[nbj] = j

		already_taken[j] = True 

	return sol

def generateRndSol(instance):
	"""
	Fill the solution with numbers between 0 and nb_jobs, shuffled
	""" 
	sol = [i for i in range(instance.getNbJob())]
	random.shuffle(sol)
	return {"jobs": sol, "wct": instance.computeWCT3(sol)}

def getBestSubset(size, job, start, instance):
	# print("subset size : ", size)
	subset = start
	best_subset = []
	best_wct = 10000000000
	for i in range(size):
		subset.insert(i, job)

		temp_instance = pfsinstance.PfspInstance(len(subset), instance.getNbMac())
		temp_instance.setProcessingTimes(instance.getProcessingTimes())
		temp_instance.setWeights(instance.getWeights())
		wct = temp_instance.computeWCT3(subset)

		# print(">", subset, wct)
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
	
	# print("weighted sums: ", weighted_sums, len(weighted_sums))
	# print("starting sequence : ", starting_sequence, len(starting_sequence))

	best_sol = {"jobs":[], "wct": 100000000000} # step 1
	for j in range(len(starting_sequence)): # (job, T)
		# print("Step {0} = {1} with J{2}".format(j, best_sol, starting_sequence[j][0]))

		bests = getBestSubset(j+1, starting_sequence[j][0], best_sol["jobs"], instance)
		best_sol["jobs"] = bests[0]
		best_sol["wct"] = bests[1]
		# print("best : ", best_sol)


	return best_sol

def generateInitialSolution(mode, instance):
	if (mode == "R"):
		return generateRndSol(instance)
	elif (mode == "SRZ"):
		return simplifiedRZheuristic(instance)

def swapValues(temp_sol, i, j):
	# swap elements at position i and i + 1
	temp_val = temp_sol[i]
	temp_sol[i] = temp_sol[j]
	temp_sol[j] = temp_val 

def getBestTransposeNeighbour(sol, instance, pivoting_rule):
	"""
	Two permutations ø, ø' are transpose neighbours if, and only if, 
	one can be obtained from the other by swapping the positions of two adjacent jobs
	ex : [A,B,C,D,E,F]
	=>   [A,C,B,D,E,F]
	"""
	best_sol = {"jobs": [], "wct": sol["wct"]}
	# print("sol init : ", sol)

	for i in range(0, len(sol["jobs"])-1):
		temp_sol = copy.copy(sol["jobs"])
		swapValues(temp_sol, i, i+1)
		wct = instance.computeWCT3(temp_sol)
		if (wct < best_sol["wct"]):
			best_sol["jobs"] = copy.copy(temp_sol)
			best_sol["wct"] = wct
			if (pivoting_rule == "FI"): # stop directly after 1 sol found
				return best_sol

		# print(temp_sol, wct)

	return best_sol

def getBestExchangeNeighbour(sol, instance, pivoting_rule):
	"""
	Two permutations ø, ø' are 2-exchange neighbours if, and only if, 
	one can be obtained from the other by exchanging two jobs at arbitrary positions
	ex : [A,B,C,D,E,F]
	=>   [A,E,C,D,B,F]
	"""
	best_sol = {"jobs": [], "wct": sol["wct"]}
	# print("sol init : ", sol)

	for i in range(len(sol["jobs"])):
		for j in range(len(sol["jobs"])):
			if (i != j):
				temp_sol = copy.copy(sol["jobs"])
				swapValues(temp_sol, i, j)
				wct = instance.computeWCT3(temp_sol)
				if (wct < best_sol["wct"]):
					best_sol["jobs"] = copy.copy(temp_sol)
					best_sol["wct"] = wct
					if (pivoting_rule == "FI"): # stop directly after 1 sol found
						return best_sol

				# print(temp_sol, wct)
	return best_sol

def getBestInsertionNeighbour(sol, instance, pivoting_rule):
	"""
	Two permutations ø, ø' are insertion neighbours if, and only if, 
	one can be obtained from the other by removing a job from one position 
	and inserting it at another position
	ex : [A,B,C,D,E,F]
	=>   [A,C,D,B,E,F]

	QUESTION : l'insertion peuut se faire en fin de liste aussi ? 
	"""
	best_sol = {"jobs": [], "wct": sol["wct"]}
	# print("sol init : ", sol)

	for i in range(len(sol["jobs"])):
		for j in range(len(sol["jobs"])+1):
			if (i != j and j != i+1): #to avoid useless solutions
				temp_sol = copy.copy(sol["jobs"])
				temp_sol.insert(j, sol["jobs"][i])
				if (j < i):
					temp_sol.pop(i+1)
				else:
					temp_sol.pop(i)

				wct = instance.computeWCT3(temp_sol)
				if (wct < best_sol["wct"]):
					best_sol["jobs"] = copy.copy(temp_sol)
					best_sol["wct"] = wct
					if (pivoting_rule == "FI"): # stop directly after 1 sol found
						return best_sol

				# print(temp_sol, wct)
				
	return best_sol

def isLocalOptimal(sol):
	"""
	Check if the solution is a local optimum. By definition, a local optimum (here minimum) is :
	search position without improving neighbours w.r.t. given evaluation function g and neighbourhood
	N. i.e. position s in S such that g(s) <= g(s') for all s' of N(s). 
	=> same as checking if the neighbour found is empty. (if empty : sol is a local optimum)
	"""
	return sol["jobs"] == [] # if true => local optimum

def chooseNeighbour(sol, instance, neighbour_type, pivoting_rule):
	"""
	Choose one neighbour (the best one) among all the neighbours of a given solution. 
	"""
	if (neighbour_type == "T"):
		return getBestTransposeNeighbour(sol, instance, pivoting_rule) 
	elif (neighbour_type == "E"):
		return getBestExchangeNeighbour(sol, instance, pivoting_rule)
	elif (neighbour_type == "I"):
		return getBestInsertionNeighbour(sol, instance, pivoting_rule)

def iterativeImprovement(args, instance):
	"""
	Heuristic algorithm to find the optimal solution to the PSFP
	args = [name.py, initsol, pivotingrule, neighbourhood]
	"""
	solution = generateInitialSolution(args[1], instance) # dictionary
	neighbour = chooseNeighbour(solution, instance, args[3], args[2])
	it = 0
	while not isLocalOptimal(neighbour):
		solution["jobs"] = copy.copy(neighbour["jobs"])
		solution["wct"] = neighbour["wct"]
		# print("it: {0} | {1} : {2}".format(it, solution["jobs"], solution["wct"]))
		neighbour = chooseNeighbour(solution, instance, args[3], args[2])
		it += 1

	return solution

def variableNeighbourhoodDescent(initsolRule, neighbourhoods, instance):
	"""
	VND variable neighbourhood descent
	use different neighbourhood mode throughout the algorithm
	args = [name.py, initsol, pivotingrule, neighbourhood]
	"""

	solution = generateInitialSolution(initsolRule, instance) # dictionary
	i = 0
	it = 0
	while i > len(neighbourhoods):
		print("neighbourhood mode : ", n)
		neighbour = chooseNeighbour(solution, instance, neighbourhoods[i], "FI")
		if (isLocalOptimal(neighbour)): # if no existing improving solution
			i += 1
		else:
			solution["jobs"] = copy.copy(neighbour["jobs"])
			solution["wct"] = neighbour["wct"]
			print("it: {0} | {1} : {2}".format(it, solution["jobs"], solution["wct"]))
			i = 0
			it += 1

	return solution

def averageRelativePercentageDev(deltas):
	"""
	Returns the average relative percentage deviation of all instances of algorithm k
	algorithm k = a combination of different mode
		for example : --random --firstImprovement --tranpose
	"""
	return sum(delta) / len(delta) 


def readBestKnownWCT(filename):
	"""
	Read the instance best known WCT file
	filename = bestSolutions.txt
	"""

	best_knowns = {}
	try:
		with open(filename) as fileIn:
			lines = fileIn.readlines()[1:] # we ignore the first line 
			for line in lines:
				l = line.strip().split(" , ")
				best_knowns[l[0]] = float(l[1]) # filename : best score

	
	except FileNotFoundError:
		print("File does not exist : ", filename)
	except:
		print("Other error")

	return best_knowns

