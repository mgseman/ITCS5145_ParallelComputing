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

# bubblesort sequential
for n in ${BUBBLESORT_NS} ; 
do
  ./bubblesort_seq ${n}  >/dev/null 2> ${RESULTDIR}/bubblesort_seq_${n}
done

