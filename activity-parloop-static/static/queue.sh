#!/bin/bash


RESULTDIR=result/
h=`hostname`



if [ ! -e .passed_static ];
then
    echo "You must pass all test before submitting"
    exit 1
fi

if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi
    

# $THREADS
source ../params.sh


for thread in ${THREADS};
do
	sbatch --partition=Centaurus --chdir=`pwd` --export=THREAD=${thread} --time=02:00:00 --ntasks=1 --cpus-per-task=${thread} --job-name=mod4ps run_static.sh
done 
