#!/bin/bash

if [ ! -e .passed_prefix ] ;
then
    echo "Must pass \`make test\` before queueing jobs"
    exit 1
fi


#running=$(qstat -u ${USER} | egrep "bench_prefixsum.*[R|Q]")
#if [ -n "${running}" ] ;
#then
#    echo "------------------------------------"
#    echo ${USER} has this job in the queue or running 
#    qstat | grep ${USER}
#    echo
#    echo please delete this job or wait for it to complete
#    exit 1
#fi

sbatch --partition=Centaurus --chdir=`pwd` --time=01:00:00 --ntasks=1 --cpus-per-task=16 --mem=40G --job-name=mod4pps bench_prefixsum.sh


echo "-----------------------"
echo
echo "Once the job COMPLETES, plot with \`make plot\`"
