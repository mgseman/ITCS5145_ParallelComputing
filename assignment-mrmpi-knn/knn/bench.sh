#!/bin/bash

RESULTDIR=result/

if [ ! -d ${RESULTDIR} ];
then
   mkdir ${RESULTDIR}
fi

. ../params.sh


P=${SLURM_CPUS_ON_NODE}
NP=$(expr ${SLURM_NTASKS} / ${SLURM_CPUS_ON_NODE})

DBFILE=/projects/class/itcs5145_001/knn_instances/waveform-5000.db
QUERYFILE=/projects/class/itcs5145_001/knn_instances/waveform-5000.query

TIMEFILE=${RESULTDIR}/knn_mrmpi_${NP}_${P}

mpirun ./knn_mrmpi ${DBFILE} ${QUERYFILE} 5 2> ${TIMEFILE}

process_time_file ${TIMEFILE}
