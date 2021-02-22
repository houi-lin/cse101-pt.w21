#!/bin/bash
# cse101-pt.w21
# usage: pa6.sh
# (run within your pa6 directory to test your code)

SRCDIR=https://raw.githubusercontent.com/houi-lin/cse101-pt.w21/master/pa6

EXE_ALL=( pa6-func-check.sh pa6-func2-check.sh pa6-unit-check.sh pa6-make-check.sh )
EXE_RANGE=$((${#EXE_ALL[*]} - 1))
echo $EXE_RANGE
for i in $(seq 0 $EXE_RANGE); do
  curl $SRCDIR/${EXE_ALL[i]} > ${EXE_ALL[i]}
  chmod +x ${EXE_ALL[i]}
  ./${EXE_ALL[i]}
  rm -f ${EXE_ALL[i]}
done
