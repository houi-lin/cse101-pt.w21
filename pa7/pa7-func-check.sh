#!/usr/bin/bash

SRCDIR=https://raw.githubusercontent.com/houi-lin/cse101-pt.w21/master/pa7
NUMTESTS=5
PNTSPERTEST=4
let MAXPTS=$NUMTESTS*$PNTSPERTEST
TIME=30

if [ ! -e backup ]; then
  mkdir backup
fi

cp *.c *.h Makefile backup   # copy all files of importance into backup

for NUM in $(seq 1 $NUMTESTS); do
  curl $SRCDIR/infile$NUM.txt > infile$NUM.txt
  curl $SRCDIR/model-outfile$NUM.txt > model-outfile$NUM.txt
  rm -f outfile$NUM.txt
done


rm -f *.o Arithmetic

gcc -c -Wall -std=c11 -g Arithmetic.c BigInteger.c List.c -lm
gcc -o Arithmetic Arithmetic.o BigInteger.o List.o -lm


echo ""
echo ""

passed=$(expr 0)
echo "Please be warned that the following tests discard all output to stdout/stderr"
echo "Arithmetic tests: If nothing between '=' signs, then test is passed"
echo "Press enter to continue"
read verbose
for NUM in $(seq 1 $NUMTESTS); do
  rm -f outfile$NUM.txt
  timeout "${TIME}" Arithmetic infile$NUM.txt outfile$NUM.txt &> garbage >> garbage #all stdout/stderr thrown away
  diff -bBwu outfile$NUM.txt model-outfile$NUM.txt > diff$NUM.txt &>> diff$NUM.txt
  echo "Arithmetic Test $NUM:"
  echo "=========="
  cat diff$NUM.txt
  echo "=========="
  read verbose
  if [ -e diff$NUM.txt ] && [[ ! -s diff$NUM.txt ]]; then # increment number of tests passed counter
    let passed+=1
  fi
done

let testspoints=$PNTSPERTEST*$passed
if [ "$testspoints" -gt "$MAXPTS" ]; then # max 10 points
  let testspoints=$(expr $MAXPTS)
fi
echo "Passed $passed Arithmetic tests for a total of $testspoints / $MAXPTS points"

echo ""
echo ""

rm -f *.o Arithmetic garbage diff*
