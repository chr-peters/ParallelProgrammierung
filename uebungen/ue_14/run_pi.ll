#! /bin/bash -x
#SBATCH --nodes=2
#SBATCH --ntasks=2
#SBATCH --cpus-per-task=24
#SBATCH --output=pi-out.%j
#SBATCH --error=pi-err.%j
#SBATCH --time=00:01:00
#SBATCH --partition=batch

srun pi_parallel.exe