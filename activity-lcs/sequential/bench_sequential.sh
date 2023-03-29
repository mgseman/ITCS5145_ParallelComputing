#!/bin/bash

RESULTDIR=result/
h=`hostname`

if [ "$h" = "gal-i1.uncc.edu"  ];
then
    echo Do not run this on the headnode of the cluster, use qsub!
    exit 1
fi

if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi

# import params
source ../params.sh

# longest common subsequence sequential
for m in ${LCS_NS};
do
    for n in ${LCS_NS};
    do 
        if [ "${m} -le "${n} ] ; then
            ./lcs_seq ${m} ${n} >/dev/null 2> ${RESULTDIR}/lcs_seq_${m}_${n}
        fi
    done
done

