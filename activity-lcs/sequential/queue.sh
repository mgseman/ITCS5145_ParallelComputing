#!/bin/bash

sbatch --partition=Centaurus --chdir=`pwd` --time=02:00:00 --ntasks=1 --cpus-per-task=1 --mem=105G --job-name=mod4slcs bench_sequential.sh
