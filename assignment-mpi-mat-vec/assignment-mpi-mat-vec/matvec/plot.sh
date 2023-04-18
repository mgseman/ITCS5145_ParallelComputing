#!/bin/bash

RESULTDIR=result/
PLOTDIR=plots/
h=`hostname`


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
for NP in ${MAT_MUL_NP};
do
   for N in ${MAT_MUL_NS};
   do

      # skip NS to large to fit on 1 node
      if [[ "${N}" -gt "100000" && ${NP} -eq "1" ]]; 
      then
         continue
      fi

      for ITER in ${ITERS};
      do
         if [ ! -s ${RESULTDIR}/mpi_matmul_${N}_${ITER}_${NP} ] ;
         then
            echo ERROR: ${RESULTDIR}/mpi_matmul_${N}_${ITER}_${NP} not found 
            echo run \'make bench\'  and WAIT for it to complete
            exit 1
         fi
      done
   done
done


	     
# format output
if false; then
    for NP in ${MAT_MUL_NP};
    do
	for P in ${PS};
	do
	    
	    NPP=$(expr ${NP} \* ${P})
	    if [ "${NPP}" -le "32" ] ;
	    then
		# format 'N seq para'
		for ITER in ${ITERS};
		do
		    for N in ${MAT_MUL_NS};
		    do
			echo ${N} \
			     $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_1_1)\
			     $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_${NP}_${P})
		    done  > ${RESULTDIR}/speedupn_${NP}_${P}_${ITER}
		done
		
		# format 'ITER seq para'
		for N in ${MAT_MUL_NS};
		do
		    for ITER in ${ITERS};
		    do
			echo ${ITER} \
			     $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_1_1)\
			     $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_${NP}_${P})
		    done  > ${RESULTDIR}/speedup_inten_${NP}_${P}_${N}
		done
		
	    fi
	done
    done
fi

for NP in ${MAT_MUL_NP};
do

   for ITER in ${ITERS};
   do
      # format 'N seq para'
      for N in ${MAT_MUL_NS};
      do
         if [ "${N}" -lt "100000" ] ; 
         then
            echo ${N} \
	         $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_1)\
		 $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_${NP})
         fi
      done  > ${RESULTDIR}/speedup_n_${NP}_${ITER}
   done

   for N in ${MAT_MUL_NS};
   do
      if [ "${N}" -lt "100000" ] ; 
      then
      # format 'N seq para'
         for ITER in ${ITERS};
         do
            echo ${ITER} \
	         $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_1)\
		 $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_${NP})
         done  > ${RESULTDIR}/speedup_iter_${NP}_${N}
      fi
   done
done

for NP in ${MAT_MUL_NP};
do

   for ITER in ${ITERS};
   do
       #format N flops
       for N in ${MAT_MUL_NS};
      do
         if [ "${N}" -lt "100000" ] || [ "${NP}" != "1" ] ; 
         then
	     TIME=$(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_${NP})
	     FLOP=$(echo ${N} \* ${N} \* ${ITER} \* 2 | bc -l)
	     FLOPS=$(echo ${FLOP} / ${TIME}  | bc -l)

	     echo ${N} ${FLOPS}		 
         fi
      done  > ${RESULTDIR}/flops_n_${NP}_${ITER}
   done
done

# plot


for ITER in ${ITERS};
do
    GSFLOP="${GSFLOP} ; set title 'ITER=${ITER}' ; plot "
    for NP in ${MAT_MUL_NP};
    do
	GSFLOP="${GSFLOP} '${RESULTDIR}/flops_n_${NP}_${ITER}' u 1:(\$2 / 1000/1000/1000) t 'P=${NP}x${NP}', "
	
   done
done
for ITER in ${ITERS};
do
   GSPNP="${GSPNP} ; set title 'ITER=${ITER}' ; plot "
   color=1
   for NP in ${MAT_MUL_NP};
   do
      GSPNP="${GSPNP} '${RESULTDIR}/speedup_n_${NP}_${ITER}' u 1:(\$2/\$3) t 'nodes x core per: ${NP}x${NP}' lc ${color} lw 3,  "
      color=`expr $color + 1`
   done
done

for NP in ${MAT_MUL_NP} ;
do
   GSPIT="${GSPIT} ; set title 'Number of Nodes=${NP}' ; plot "
   color=1
   for N in ${MAT_MUL_NS};
   do
      if [ "${N}" -lt "100000" ] ; 
      then
         GSPIT="${GSPIT} '${RESULTDIR}/speedup_iter_${NP}_${N}' u 1:(\$2/\$3) t 'N=${N}' lc ${color} lw 3,  "
         color=`expr $color + 1`
      fi
   done
done
#
# creat time tables

cd plots


cd ..


#gnuplot <<EOF

#set terminal pdf
#set output '${PLOTDIR}matmul_N_plots.pdf'
#set style data linespoints
#set key top left;
#set xlabel 'N'; 
#set ylabel 'speedup';
#set xrange [*:*];
#set logscale x 10;
#set yrange [0:36];
#set ytics 2;

#${GSPNP}

#EOF


gnuplot <<EOF

set terminal pdf
set output '${PLOTDIR}matmul_ITER_plots.pdf'
set style data linespoints
set key top left;
set xlabel 'ITER'; 
set ylabel 'speedup';
set xrange [1:5];
set yrange [0:36];
set ytics 2;

${GSPIT}

EOF

gnuplot <<EOF

set terminal pdf
set output '${PLOTDIR}matmul_FLOP_plots.pdf'
set style data linespoints
set key top left;
set xlabel 'N'; 
set ylabel 'GFlops';

${GSFLOP}

EOF
