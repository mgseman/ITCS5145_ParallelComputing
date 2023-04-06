#!/bin/bash

RESULTDIR=result/
h=`hostname`

if [ "$h" = "gal-i1.uncc.edu"  ];
then
    echo Do not run this on the headnode of the cluster, use qsub!
    exit 1
fi

# create result directory
if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi


# import params
source ../params.sh

echo starting time is $(date) 

# prefixsum sequential
for n in ${PREFIXSUM_NS};
do 
    ./prefixsum_seq ${n} >/dev/null 2> ${RESULTDIR}/prefixsum_seq_${n}
done



echo ending time is $(date) 

