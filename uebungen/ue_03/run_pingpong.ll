#! /bin/bash -x
#SBATCH --nodes=1
#SBATCH --ntasks=2
#SBATCH --output=pingpong-out.%j
#SBATCH --error=pingpong-err.%j
#SBATCH --time=00:01:00
#SBATCH --partition=batch

MODE=2
I=0
while [ "$I" -lt "22" ]
do
    size=`echo "2^$I" | bc`
    srun pingpong.exe $size $MODE
    #echo $size
    I=`echo "$I + 1" | bc`
done
