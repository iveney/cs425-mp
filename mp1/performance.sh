#!/bin/bash

UNAME=`uname`
if [[ "$UNAME" == "Linux" ]];then
  TIME=/usr/bin/time
elif [[ "$UNAME" == "Darwin" ]];then
  TIME=gtime
else
  echo "Unrecognized system"
  exit 1
fi

REPEAT=10
CASE=("rare" "somewhat" "frequent")
PATTERN=("zigang" "key" "e")
LOGS="machine.1.log machine.2.log machine.3.log machine.4.log"
# LOGS="machine.0.log"

for i in `seq 0 2`
do
  testcase=${CASE[$i]}
  pattern=${PATTERN[$i]}
  total=0
  for t in `seq 1 $REPEAT`
  do
    # because time output to stderr
    elapsed=$( ($TIME -f "%e" ./dgrep -p "$pattern" $LOGS > /dev/null) 2>&1 )
    printf "%8s (%2d): %s\n" $testcase $t $elapsed
    total=`echo "$total + $elapsed" | bc` 
  done
  avg=`echo "scale=2; $total / $REPEAT.0" | bc`
  echo "Avg($testcase) = $avg"
done

