#!/bin/sh

sbatch --partition=GPU --chdir=`pwd` --time=04:00:00 --ntasks=1 --cpus-per-task=16 --gpus-per-task=1 --mem=100G --job-name=mod8tgp --output="gputest-%A.out" test_gpu.sh
