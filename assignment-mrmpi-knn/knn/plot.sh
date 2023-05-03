#!/bin/bash

RESULTDIR=result/
PLOTDIR=plots/
#h=`hostname` #not necessary


# create result directory
if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi

# create plot directory
if [ ! -d ${PLOTDIR} ];
then
    mkdir ${PLOTDIR}
fi

# import params
. ../params.sh


# error checking
#   file existence
#   file populate
for NP in ${NODES};
do
   for P in ${PS_PER_NODE};
   do
      NPP=$(expr ${NP} \* ${P})
      if [ "${NPP}" -le "32" ] ;
      then
        if [ ! -s ${RESULTDIR}/knn_mrmpi_${NP}_${P} ] ;
        then
          echo "ERROR: ${RESULTDIR}/knn_mrmpi_${NP}_${P} not found"
          echo "run \'make bench\'  and WAIT for it to complete"
          exit 1
        fi
      fi
   done
done

	     

# format 'N seq para'
for NP in ${NODES};
do
   for P in ${PS_PER_NODE};
   do
      NPP=$(expr ${NP} \* ${P})
      if [ "${NPP}" -le "32" ] ;
      then
        echo ${P} \
        $(cat ${RESULTDIR}/knn_mrmpi_1_1)\
        $(cat ${RESULTDIR}/knn_mrmpi_${NP}_${P})
      fi
    done  > ${RESULTDIR}/speedup_nodes_${NP}
done

for P in ${PS_PER_NODE};
do
   for NP in ${NODES};
   do
      NPP=$(expr ${NP} \* ${P})
      if [ "${NPP}" -le "32" ] ;
      then
        echo ${NP} \
        $(cat ${RESULTDIR}/knn_mrmpi_1_1)\
        $(cat ${RESULTDIR}/knn_mrmpi_${NP}_${P})
      fi
   done  > ${RESULTDIR}/speedup_pernode_${P}
done


# plot


GSPNP="${GSPNP} ; set title '' ; plot "
color=1
for NP in ${NODES};
do
   GSPNP="${GSPNP} '${RESULTDIR}/speedup_nodes_${NP}' u 1:(\$2/\$3) t 'Num Nodes=${NP}' lc ${color} lw 3,  "
   color=`expr $color + 1`
done

GSPP="${GSPP} ; set title '' ; plot "
color=1
for P in ${PS_PER_NODE};
do
   GSPP="${GSPP} '${RESULTDIR}/speedup_pernode_${P}' u 1:(\$2/\$3) t 'Cores per node=${P}' lc ${color} lw 3,  "
   color=`expr $color + 1`
done
 


gnuplot <<EOF

set terminal pdf
set output '${PLOTDIR}KNN_NODES_plots.pdf'
set style data linespoints
set key top left;
set key samplen 2 spacing 1 font ",8"
set xlabel 'Cores per node'; 
set ylabel 'speedup';
set xrange [0:18];
set yrange [0:36];
set ytics 2;
set xtics 1;

${GSPNP}

EOF

gnuplot <<EOF

set terminal pdf
set output '${PLOTDIR}KNN_PER_NODE_plots.pdf'
set style data linespoints
set key top right;
set key samplen 2 spacing 1 font ",8"
set xlabel 'Number of Nodes'; 
set ylabel 'speedup';
set xrange [0:6];
set yrange [0:36];
set ytics 2;
set xtics 1;

${GSPP}

EOF

