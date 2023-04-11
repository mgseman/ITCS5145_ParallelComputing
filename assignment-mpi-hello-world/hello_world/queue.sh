#!/bin/bash

sbatch --partition=Centaurus --chdir=`pwd` --time=00:05:00 --nodes=1 --ntasks-per-node=16 --job-name=mod6hwd ./bench.sh
sbatch --partition=Centaurus --chdir=`pwd` --time=00:05:00 --nodes=2 --ntasks-per-node=8 --job-name=mod6hwd ./bench.sh
sbatch --partition=Centaurus --chdir=`pwd` --time=00:05:00 --nodes=4 --ntasks-per-node=8 --job-name=mod6hwd ./bench.sh

