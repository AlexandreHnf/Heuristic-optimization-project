class PfspInstance:


	def __init__(self, nb_jobs=0, nb_mach=0):
		self.nb_jobs = nb_jobs
		self.nb_mach = nb_mach 
		self.due_dates = []
		self.weights = []
		self.processing_times = []
		self.wct = 0 # weighted sum of completion times
		self.makespan = 0 # total time taken
		self.sct = 0 # sum of completion times 
		if (self.nb_jobs > 0 and self.nb_mach > 0):
			self.allowMatrixMemory()
		self.best_known_wct = 0

		# print("nb jobs: ", self.nb_jobs)
		# print("nb mach: ", self.nb_mach)
		# print(self.processing_times)

	def getNbJob(self):
		return self.nb_jobs

	def getNbMac(self):
		return self.nb_mach

	def allowMatrixMemory(self):
		self.processing_times = [[0 for _ in range(self.nb_mach)] for _ in range(self.nb_jobs)]
		self.due_dates = [0 for _ in range(self.nb_jobs)]
		self.weights = [0 for _ in range(self.nb_jobs)]

	def getTime(self, job, machine):
		if ((job < 0) or (job > self.nb_jobs) or (machine < 0) or (machine > self.nb_mach)):
			print("ERROR. file:pfspInstance.py, method:getTime. Out of Bound. Job={0}, machine={1}".format(job, machine)) 
		else:
			return self.processing_times[job][machine]


	def setTime(self, job, machine, process_time):
		self.processing_times[job][machine] = process_time

	def getDueDate(self, job):
		return self.due_dates[job] 

	def setDueDate(self, job, due_date):
		self.due_dates[job] = due_date 

	def getWeight(self, job):
		return self.weights[job] 

	def setWeight(self, job, prior):
		self.weights[job] = prior 

	def getWeights(self):
		return self.weights 
	
	def setWeights(self, w):
		self.weights = w

	def getSubsetJobs(self, jobs):
		subset = []
		for j in jobs:
			subset.append(self.processing_times[j])
		return subset

	def getProcessingTimes(self):
		return self.processing_times

	def setProcessingTimes(self, p):
		self.processing_times = p

	def readDataFromFile(self, filename):
		"""
		Read instance file
		"""
		everythingOK = True

		try:
			#trying to open a file in read mode
			with open(filename) as fileIn:
				data = fileIn.readlines()
				# print(data)
				sizes = data[0].strip().split()
				self.nb_jobs = int(sizes[0])
				self.nb_mach = int(sizes[1])
				# print("number of jobs : ", self.nb_jobs)
				# print("number of machines : ", self.nb_mach)
				self.allowMatrixMemory()
				# print(self.processing_times)

				for i in range(0, self.nb_jobs):
					line = data[i+1].strip().split() #+1 because 0 is "nbJ nbMM"
					# print(line)
					for j in range(1, self.nb_mach*2, 2): # every two values to avoid mach nb
						mach = (j-1)//2
						# print(i, j, j-1, mach, line[j])
						self.processing_times[i][mach] = int(line[j])

				offset = self.nb_jobs + 2
				for j in range(self.nb_jobs): # from after 'Reldue' to the end
					# print("j::", j+offset)
					date_and_prior = data[j+offset].strip().split() # [-1, 1898, -1, 4]
					# print(date_and_prior)
					self.due_dates[j] = int(date_and_prior[1]) # due date
					self.weights[j] = int(date_and_prior[3]) # priority

			print("All is read from file")
			# print(self.processing_times)

		except FileNotFoundError:
			print("File does not exist : ", filename)
			everythingOK = False
		except:
			print("Other error")
	   
		return everythingOK

	def setBestKnownWCT(self, best_known):
		"""
		set best known WCT from the file 'bestSolutions.txt'
		"""

		self.best_known_wct = best_known

	def getBestKnownWCT(self):
		return self.best_known_wct

	def getRelativePercentageDeviation(self, best_sol):
		"""
		Computes the relative percentage deviation of the algorithm k on the instance i
		algorithm k = a combination of different mode
		for example : --random --firstImprovement --tranpose
		"""

		return 100 * ((best_sol["wct"] - self.best_known_wct) / self.best_known_wct)

	def computeWCT(self, sol):
		"""
		Compute Weighted Completion Time of a solution
		"""
		job_nb = 0

		#We need end times on previous machine 
		prev_mach_endtime = [None for _ in range(self.nb_jobs)] # length = nb_jobs
		prev_job_endtime = 0 

		# first machine :
		prev_mach_endtime[0] = 0
		for j in range(self.nb_jobs):
			job_nb = sol[j]
			prev = 0 # if 1st job previous elements (= None)
			if (j-1 > -1):
				prev = prev_mach_endtime[j-1]
			prev_mach_endtime[j] = prev + self.processing_times[job_nb][0]

		for m in range(1, self.nb_mach):
			prev_mach_endtime[0] += self.processing_times[sol[0]][m]
			prev_job_endtime = prev_mach_endtime[0]
			for j in range(1, self.nb_jobs):
				job_nb = sol[j]
				if (prev_mach_endtime[j] > prev_job_endtime):
					prev_mach_endtime[j] = prev_mach_endtime[j] + self.processing_times[job_nb][m]
					prev_job_endtime = prev_mach_endtime[j]
				else:
					prev_job_endtime += self.processing_times[job_nb][m]
					prev_mach_endtime[j] = prev_job_endtime

		wct = 0
		for j in range(self.nb_jobs):
			wct += prev_mach_endtime[j] * self.weights[sol[j]]

		self.wct = wct 
		self.sct = sum(prev_mach_endtime)
		self.makespan = prev_mach_endtime[-1] # last completion time 

		return wct 

	def weightedSumSingleJob(self, job):
		"""
		Computes the weighted sum of processing times of job Ji 
		"""
		ws = 0
		for j in range(self.nb_mach):
			ws += (self.processing_times[job][j]) / self.weights[job]
		return ws 

	def showStats(self):
		print("---------------------------------------")
		print("Weighted sum completion Time : ", self.wct)
		print("makespan : ", self.makespan)
		print("Sum of completion time : ", self.sct)
		print("---------------------------------------")

def main():
	p = PfspInstance()
	filename = "D:\\Users\\Alexandre\\Desktop\\ULB\\MA2\\Heuristic optimization\\Projet\\repository\\Heuristic-optimization-project\\instances\\50_20_01"
	status = p.readDataFromFile(filename)
	print("status: ", status)



	# ======== test WCT ========== :
	


if __name__ == "__main__":
	main()