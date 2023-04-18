#!/bin/bash

sbatch --partition=Centaurus --chdir=`pwd` --time=00:05:00 --nodes=1 --ntasks-per-node=2 --job-name=mod6pp ./bench.sh
