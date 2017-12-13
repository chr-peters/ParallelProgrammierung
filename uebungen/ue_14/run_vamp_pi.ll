#! /bin/bash -x
#SBATCH --nodes=1
#SBATCH --ntasks=24
#SBATCH --output=pi-out.%j
#SBATCH --error=pi-err.%j
#SBATCH --time=00:01:00
#SBATCH --partition=batch

export SCOREP_ENABLE_TRACING=true
srun pi_serial.exe
