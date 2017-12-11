#! /bin/bash -x
#SBATCH --nodes=2
#SBATCH --ntasks=16
#SBATCH --output=cartesian_communicators-out.%j
#SBATCH --error=cartesian_communicators-err.%j
#SBATCH --time=00:01:00
#SBATCH --partition=batch

srun cartesian_communicators.exe
