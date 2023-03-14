#!/bin/bash

sbatch --partition=Centaurus --chdir=`pwd` --time=02:00:00 --ntasks=1 --cpus-per-task=1 --job-name=mod4pfs bench_sequential.sh
