#!/bin/bash

RESULTFILE=result_gpu

rm ${RESULTFILE}

./cuda_vectoradd 1 | tee -a ${RESULTFILE}
./cuda_vectoradd 10 | tee -a ${RESULTFILE}
./cuda_vectoradd 100 | tee -a ${RESULTFILE}
./cuda_vectoradd 1000 | tee -a ${RESULTFILE}
./cuda_vectoradd 10000 | tee -a ${RESULTFILE}
./cuda_vectoradd 100000 | tee -a ${RESULTFILE}
./cuda_vectoradd 1000000 | tee -a ${RESULTFILE}
./cuda_vectoradd 10000000 | tee -a ${RESULTFILE}
./cuda_vectoradd 100000000 | tee -a ${RESULTFILE}
./cuda_vectoradd 1000000000 | tee -a ${RESULTFILE}
