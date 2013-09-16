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

SET=10M
REPEAT=10
CASE=("rare" "somewhat" "frequent")
PATTERN=("zigang" "two" "e")
LOGS="machine.1.log machine.2.log machine.3.log machine.4.log"
#LOGS="machine.5.log"

for i in `seq 0 2`
do
  testcase=${CASE[$i]}
  pattern=${PATTERN[$i]}
  total=0
  list=()
  for t in `seq 1 $REPEAT`
  do
    # because time output to stderr
    elapsed=$( ($TIME -f "%e" ./dgrep -p "$pattern" $LOGS > /dev/null) 2>&1 )
    list=( ${list[@]} $elapsed )
    printf "%8s (%2d): %s\n" $testcase $t $elapsed
    total=`echo "$total + $elapsed" | bc` 
  done
  avg=`echo "scale=2; $total / $REPEAT" | bc`
  echo "Avg($testcase) = $avg"
  echo "${list[@]}" > $SET.$testcase.time
done

