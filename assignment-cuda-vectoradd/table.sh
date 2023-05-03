#!/bin/bash

echo n cputime gputime gpuspeedup

join result_cpu result_gpu | cut -f 1,2,3 -d \  | awk '{print $1, $2, $3, $2/$3}' | tee resulttable.txt
