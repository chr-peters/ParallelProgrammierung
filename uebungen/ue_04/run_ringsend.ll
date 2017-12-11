#! /bin/bash -x
#SBATCH --nodes=1
#SBATCH --ntasks=8
#SBATCH --output=ringsend-out.%j
#SBATCH --error=ringsend-err.%j
#SBATCH --time=00:01:00
#SBATCH --partition=batch

srun ringsend.exe
