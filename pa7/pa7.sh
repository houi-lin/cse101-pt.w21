#!/bin/bash
# cmps101-pt.w21 grading
# usage: pa7.sh
# (run within your pa7 directory to test your code)

SRCDIR=https://raw.githubusercontent.com/houi-lin/cse101-pt.w21/master/pa7

EXE_ALL=( pa7-func-check.sh pa7-lunit-check.sh pa7-bunit-check.sh pa7-make-check.sh )
EXE_RANGE=$((${#EXE_ALL[*]} - 1))
echo $EXE_RANGE
for i in $(seq 0 $EXE_RANGE); do
  curl $SRCDIR/${EXE_ALL[i]} > ${EXE_ALL[i]}
  chmod +x ${EXE_ALL[i]}
  ./${EXE_ALL[i]}
  rm -f ${EXE_ALL[i]}
done
