#!/bin/bash

if [ ! -e .passed_mpi_heat ] ;
then
   echo Must pass \`make test\` before queuing
   exit 1 
fi

# yea, uhh we not letting you submit multiple bench
# qselect -u ${USER} -N run_matmul.sh | xargs qdel

# import params
source ../params.sh

echo starting time is $(date)

#count=0
for NP in ${NODES} ;
do 
  sbatch --partition=Centaurus --chdir=`pwd` --time=04:00:00 --nodes=${NP} --ntasks-per-node=${NP} --mem=100G --job-name=mod6shm ./bench_weak.sh
  sbatch --partition=Centaurus --chdir=`pwd` --time=04:00:00 --nodes=${NP} --ntasks-per-node=${NP} --mem=100G --job-name=mod6whm ./bench_strong.sh
done 


echo ending time is $(date)
