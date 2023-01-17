#!/bin/bash

if [ ! -e .passed ];
then
    echo "You must pass all test before submitting"
    exit 1
else
  ./queue_job.sh
fi
