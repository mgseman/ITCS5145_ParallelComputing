#!/bin/bash

RESULTDIR=result/

if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi

P=${SLURM_NTASKS}

. ../params.sh

SIZE=SIZE_WEAK_${P}


for sz in ${!SIZE} ;
do
    TIMEFILE=${RESULTDIR}/weak_${sz}_${P}
    
    mpirun ${MPIRUN_PARAMS} ./mpi_heat $sz 5 2> ${TIMEFILE}

    process_time_file ${TIMEFILE}
done
