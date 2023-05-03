#!/bin/bash

RESULTFILE=result_cpu

rm ${RESULTFILE}

./cpu_vectoradd 1 | tee -a ${RESULTFILE}
./cpu_vectoradd 10 | tee -a ${RESULTFILE}
./cpu_vectoradd 100 | tee -a ${RESULTFILE}
./cpu_vectoradd 1000 | tee -a ${RESULTFILE}
./cpu_vectoradd 10000 | tee -a ${RESULTFILE}
./cpu_vectoradd 100000 | tee -a ${RESULTFILE}
./cpu_vectoradd 1000000 | tee -a ${RESULTFILE}
./cpu_vectoradd 10000000 | tee -a ${RESULTFILE}
./cpu_vectoradd 100000000 | tee -a ${RESULTFILE}
./cpu_vectoradd 1000000000 | tee -a ${RESULTFILE}


