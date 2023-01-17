#!/bin/bash


sbatch --partition=Centaurus --chdir=`pwd` --time=04:00:00 --ntasks=1 --cpus-per-task=1 --job-name=mod1numint ./run_bench.sh
