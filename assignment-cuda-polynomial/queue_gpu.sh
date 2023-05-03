#!/bin/bash

sbatch --partition=GPU --chdir=`pwd` --time=04:00:00 --ntasks=1 --cpus-per-task=16 --gpus-per-task=1 --job-name=mod8bgp --mem=100G ./bench_gpu.sh
