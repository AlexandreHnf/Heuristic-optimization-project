#!/bin/bash
#
#SBATCH --job-name=heuristic_opti
#SBATCH -o output.txt # prints
#SBATCH -e error.txt  # errors
#SBATCH --mail-type=ALL
#SBATCH --mail-user=Alexandre.Heneffe@ulb.be
#
#SBATCH --cpus-per-task=25
#SBATCH --ntasks=1
#SBATCH --time=20:00:00
#SBATCH --mem-per-cpu=100

module load gcc
make
./flowshopRUN runRTDexperiments
