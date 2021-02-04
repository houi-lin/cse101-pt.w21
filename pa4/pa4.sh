#!/bin/bash
# cse101-pt.w21
# usage: pa4.sh
# (run within your pa4 directory to test your code)

SRCDIR=https://raw.githubusercontent.com/houi-lin/cse101-pt.w21/master/pa4

EXE_ALL=( pa4-func-check.sh pa4-unit-check.sh pa4-make-check.sh )
EXE_RANGE=$((${#EXE_ALL[*]} - 1))
echo $EXE_RANGE
for i in $(seq 0 $EXE_RANGE); do
  curl $SRCDIR/${EXE_ALL[i]} > ${EXE_ALL[i]}
  chmod +x ${EXE_ALL[i]}
  ./${EXE_ALL[i]}
  rm -f ${EXE_ALL[i]}
done
