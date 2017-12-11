#! /bin/bash -x
#SBATCH --nodes=2
#SBATCH --ntasks=32
#SBATCH --time=00:01:00
#SBATCH --partition=batch
#MSUB -o .
#MSUB -e .

for (( PROCS = 1 ; PROCS <= 4; PROCS ++ ))
do
    srun --ntasks $PROCS pi_serial.exe >> pi_speedup.dat
done
