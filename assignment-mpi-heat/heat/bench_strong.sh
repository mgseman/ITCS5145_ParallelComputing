#!/bin/bash

RESULTDIR=result/

if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi

P=${SLURM_NTASKS}

. ../params.sh

for sz in ${SIZE_STRONG};
do
    TIMEFILE=${RESULTDIR}/strong_${sz}_${P}
    
    mpirun ${MPIRUN_PARAMS} ./mpi_heat $sz 5 2> ${TIMEFILE}

    process_time_file ${TIMEFILE}
done
