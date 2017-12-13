#!/bin/bash

export OMP_NUM_THREADS=24
rm reduction.dat atomic.dat critical.dat > /dev/null 2>&1
for N in 10 100 1000 10000 100000 1000000 10000000 100000000
do
    ./pi_reduction.exe $N >> reduction.dat
    ./pi_atomic.exe $N >> atomic.dat
    ./pi_critical.exe $N >> critical.dat
done
