#!/usr/bin/bash

SRCDIR=https://raw.githubusercontent.com/houi-lin/cse101-pt.w21/master/pa6
NUMTESTS=5
PNTSPERTEST=3
let MAXPTS=$NUMTESTS*$PNTSPERTEST

if [ ! -e backup ]; then
   echo "WARNING: a backup has been created for you in the \"backup\" folder"
   mkdir backup
fi


cp *.c *.h Makefile backup   # copy all files of importance into backup

for NUM in $(seq 1 $NUMTESTS); do
   curl $SRCDIR/infile$NUM.txt > infile$NUM.txt
   curl $SRCDIR/model-outfile$NUM.txt > model-outfile$NUM.txt
done

echo ""
echo ""

gcc -c -std=c11 -Wall -g Order.c Dictionary.c
gcc -o Order Order.o Dictionary.o

lextestspassed=$(expr 0)
echo "Please be warned that the following tests discard all output to stdout while reserving stderr for valgrind output"
echo "Press enter to continue"
read verbose
for NUM in $(seq 1 $NUMTESTS); do
  rm -f outfile$NUM.txt
  timeout 15 valgrind --leak-check=full -v Order infile$NUM.txt outfile$NUM.txt &> valgrind-out$NUM.txt
  diff -bBwu outfile$NUM.txt model-outfile$NUM.txt &> diff$NUM.txt >> diff$NUM.txt
  echo "Order Test $NUM: (Press enter to continue...)"
  echo "Please check the diff# files manually if you fail the cases"
  read verbose
  echo "=========="
  if [ -e diff$NUM.txt ] && [[ ! -s diff$NUM.txt ]]; then
    echo "PASS case"
  else
    echo "FAIL"
  fi
  echo "=========="
  if [ -e diff$NUM.txt ] && [[ ! -s diff$NUM.txt ]]; then
    let lextestspassed+=1
  fi
done

let lextestpoints=${PNTSPERTEST}*lextestspassed

echo "Passed $lextestspassed / $NUMTESTS Order tests"
echo "This gives a total of $lextestpoints / $MAXPTS points"
echo ""
echo ""

echo "Press Enter To Continue with Valgrind Results for Order"
#TODO find a way to automate detecting if leaks and errors are found and how many
read garbage

for NUM in $(seq 1 $NUMTESTS); do
   echo "Order Valgrind Test $NUM: (Press enter to continue...)"
   read verbose
   echo "=========="
   cat valgrind-out$NUM.txt
   echo "=========="
done

echo ""
echo ""
rm -f *.o Order diff* valgrind-out*

