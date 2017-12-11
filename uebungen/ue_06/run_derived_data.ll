#! /bin/bash -x
#SBATCH --nodes=1
#SBATCH --ntasks=4
#SBATCH --output=derived_data-out.%j
#SBATCH --error=derived_data-err.%j
#SBATCH --time=00:01:00
#SBATCH --partition=batch

srun derived_data.exe
