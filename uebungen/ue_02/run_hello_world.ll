#! /bin/bash -x
#SBATCH --nodes=1
#SBATCH --ntasks=8
#SBATCH --output=hello-out.%j
#SBATCH --error=hello-err.%j
#SBATCH --time=00:01:00
#SBATCH --partition=batch

srun hello_world.exe