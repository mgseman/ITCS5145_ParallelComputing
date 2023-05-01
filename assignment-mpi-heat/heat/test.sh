#!/bin/bash
sci_notation_regex='^[0-9]+([.][0-9]+)?(e[0-9]+|e-[0-9]+)?$'
h=`hostname`
num_proc=`nproc`

function test_time {
    # compare 
    if [[ ! $1 =~ $sci_notation_regex ]] ; 
    then
        echo ERROR: time is not on stderr or not formatted properly
        rm .time
        exit 1
    fi
    # delete tmp file 
    rm .time
}


if [ "$h" = "gal-i1.uncc.edu"  ];
then
  SUCCESS_FILE=.passed_mpi_heat
else 
  SUCCESS_FILE=.passed_on_local
fi

. ../params.sh

NPP="1 4 16"
NS="20 32"
ITERS="1 3"
for NP in ${NPP} ;

do
    if [ ${NP} -gt ${num_proc} ]; then
     break;
    fi
   for N in ${NS} ;
   do
      for ITER in ${ITERS} ;
      do

         ANSW=$(mpirun ${MPIRUN_PARAMS} -np ${NP} ./mpi_heat ${N} ${ITER} 2> .time < /dev/null)
  
         if [ -z "${ANSW}" ] ;
         then
	     process_time_file .time
	     
            test_time $(cat .time) 
         else
            echo FAILED: "mpirum ${MPIRUN_PARAMS} -np -oversubscribe ${NP} ./mpi_heat ${N} ${ITER}"
            exit 1
         fi
      
       # "progess bar"
       echo -n "|"
      
      done
   done
done

echo
echo "All Test Succesful!"

touch ${SUCCESS_FILE}
echo
