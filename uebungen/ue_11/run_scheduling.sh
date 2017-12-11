#!/bin/bash

for sched_type in static dynamic guided
do
    for chunksize in 1 10 100 1000 10000 100000 1000000
    do
	export OMP_SCHEDULE="$sched_type,$chunksize"
	./schedule_temp.exe
    done
done
export OMP_SCHEDULE=auto
./schedule_temp.exe
