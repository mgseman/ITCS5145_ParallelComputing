#!/bin/bash

#sleep 20

. ../params.sh

echo "This scripts queues the job one at a time and needs to wait for the next job to be completed. This should only take a couple of minutes."

for N in ${NODES};
do
   for P in ${PS_PER_NODE};
   do 
      NPP=$(expr ${N} \* ${P})
      if [ "${NPP}" -le "32" ];
      then
         sbatch --partition=Centaurus --chdir=`pwd` --time=00:15:00 --nodes=${N} --ntasks-per-node=${P} --mem=100G --job-name=mod7wch ./bench.sh
      fi

      
      # ensure one job running at a time
      # This is necessary to avoid bottlenecking the IO system (Erik on 04/14/2019)
      # while [[ -n "$(qstat -u ${USER} -i)" || -n "$(qstat -u ${USER} -r)" ]];
      #do
      #  sleep 3 
      #done 

   done
done
