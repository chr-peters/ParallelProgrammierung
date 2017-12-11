#! /bin/bash -x
#SBATCH --nodes=1
#SBATCH --ntasks=8
#SBATCH --output=numarray-out.%j
#SBATCH --error=numarray-err.%j
#SBATCH --time=00:01:00
#SBATCH --partition=batch

srun numarray_2.exe
