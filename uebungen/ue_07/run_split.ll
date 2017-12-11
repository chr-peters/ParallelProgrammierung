#! /bin/bash -x
#SBATCH --nodes=1
#SBATCH --ntasks=6
#SBATCH --output=split-out.%j
#SBATCH --error=split-err.%j
#SBATCH --time=00:01:00
#SBATCH --partition=batch

srun split.exe
