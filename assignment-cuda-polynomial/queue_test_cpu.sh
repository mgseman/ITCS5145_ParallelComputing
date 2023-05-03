#!/bin/bash

sbatch --partition=Centaurus --chdir=`pwd` --time=01:00:00 --ntasks=1 --cpus-per-task=16 --mem=100G --job-name=mod8tcp --output="cputest-%A.out" ./test_cpu.sh
