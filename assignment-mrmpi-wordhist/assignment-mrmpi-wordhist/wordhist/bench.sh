#!/bin/bash

RESULTDIR=result/

if [ ! -d ${RESULTDIR} ];
then
   mkdir ${RESULTDIR}
fi

. ../params.sh


P=${SLURM_CPUS_ON_NODE}
NP=$(expr ${SLURM_NTASKS} / ${SLURM_CPUS_ON_NODE})

SDIR=/projects/class/itcs5145_001/texts/

BFILE="
${SDIR}100-0.txt ${SDIR}244-0.txt ${SDIR}46-0.txt ${SDIR}786-0.txt
${SDIR}pg19942.txt ${SDIR}pg55.txt ${SDIR}1-0.txt ${SDIR}25344-0.txt
${SDIR}47-0.txt ${SDIR}829-0.txt ${SDIR}pg20203.txt ${SDIR}pg58336.txt
${SDIR}11-0.txt ${SDIR}2554-0.txt ${SDIR}49513-0.txt ${SDIR}84-0.txt
${SDIR}pg23.txt ${SDIR}pg58343.txt ${SDIR}113-0.txt ${SDIR}2591-0.txt
${SDIR}521-0.txt ${SDIR}863-0.txt ${SDIR}pg2500.txt ${SDIR}pg58345.txt
${SDIR}1184-0.txt ${SDIR}2600-0.txt ${SDIR}56006-0.txt ${SDIR}98-0.txt
${SDIR}pg2542.txt ${SDIR}pg6130.txt ${SDIR}120-0.txt ${SDIR}2701-0.txt
${SDIR}58333-0.txt ${SDIR}pg1080.txt ${SDIR}pg2680.txt ${SDIR}pg61.txt
${SDIR}1342-0.txt ${SDIR}28054-0.txt ${SDIR}58334-0.txt ${SDIR}pg10.txt
${SDIR}pg27827.txt ${SDIR}pg685.txt ${SDIR}135-0.txt ${SDIR}2814-0.txt
${SDIR}58335-0.txt ${SDIR}pg1232.txt ${SDIR}pg28520.txt ${SDIR}pg730.txt
${SDIR}1399-0.txt ${SDIR}2852-0.txt ${SDIR}58340-0.txt ${SDIR}pg1260.txt
${SDIR}pg30360.txt ${SDIR}pg7370.txt ${SDIR}158-0.txt ${SDIR}30254-0.txt
${SDIR}58344-0.txt ${SDIR}pg1322.txt ${SDIR}pg3207.txt ${SDIR}pg768.txt
${SDIR}160-0.txt ${SDIR}35-0.txt ${SDIR}58347-0.txt ${SDIR}pg1404.txt
${SDIR}pg33.txt ${SDIR}pg779.txt ${SDIR}16-0.txt ${SDIR}3600-0.txt
${SDIR}58348-0.txt ${SDIR}pg1497.txt ${SDIR}pg345.txt ${SDIR}pg844.txt
${SDIR}1952-0.txt ${SDIR}41-0.txt ${SDIR}58349-0.txt ${SDIR}pg161.txt
${SDIR}pg34901.txt ${SDIR}pg8800.txt ${SDIR}1998-0.txt ${SDIR}4300-0.txt
${SDIR}58350-0.txt ${SDIR}pg16328.txt ${SDIR}pg408.txt ${SDIR}pg996.txt
${SDIR}203-0.txt ${SDIR}43-0.txt ${SDIR}647-0.txt ${SDIR}pg1661.txt
${SDIR}pg4363.txt ${SDIR}test.txt ${SDIR}205-0.txt ${SDIR}45-0.txt  
${SDIR}74-0.txt ${SDIR}pg174.txt ${SDIR}pg514.txt ${SDIR}219-0.txt
${SDIR}4517-0.txt ${SDIR}76-0.txt ${SDIR}pg19337.txt ${SDIR}pg5200.txt
"


TIMEFILE=${RESULTDIR}/wordhist_${NP}_${P}

mpirun ${MPIRUN_PARAMS} ./wordhist ${BFILE} 2> ${TIMEFILE}

process_time_file ${TIMEFILE}
