#!/bin/bash


if [ ! -e .passed_mpi_advancedmanagerworker ] ;
then
   echo Must pass \`make test\` before queuing
   exit 1 
fi



# yea, uhh we not letting you submit multiple bench
# qselect -u ${USER} -N run_numint.sh | xargs qdel

# import params
source ../params.sh


for NP in ${NODES} ;
do 
   for P in ${PS} ;
   do
       NPP=$(expr ${NP} \* ${P})
       if [ "$NPP" -le 32 ] ;
       then
					sbatch --partition=Centaurus --chdir=`pwd` --time=00:20:00 --nodes=${NP} --ntasks-per-node=${P} --job-name=mod6amw ./bench.sh
       fi
   done
done 

