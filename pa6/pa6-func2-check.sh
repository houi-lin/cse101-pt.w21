#!/usr/bin/bash

SRCDIR=https://raw.githubusercontent.com/houi-lin/cse101-pt.w21/master/pa6
NUMTESTS=3
PNTSPERTEST=3
let MAXPTS=$NUMTESTS*$PNTSPERTEST

if [ ! -e backup ]; then
   echo "WARNING: a backup has been created for you in the \"backup\" folder"
   mkdir backup
fi


cp *.c *.h Makefile backup   # copy all files of importance into backup

curl $SRCDIR/WordFrequency.c > WordFrequency.c
for NUM in $(seq 1 $NUMTESTS); do
   curl $SRCDIR/WF-infile$NUM.txt > WF-infile$NUM.txt
   curl $SRCDIR/Model-WF-outfile$NUM.txt > Model-WF-outfile$NUM.txt
done

echo ""
echo ""

gcc -c -std=c11 -Wall -g WordFrequency.c Dictionary.c
gcc -o WordFrequency WordFrequency.o Dictionary.o

lextestspassed=$(expr 0)
echo "WordFrequency tests: If nothing between '=' signs, then test is passed"
echo "Press enter to continue"
read verbose
for NUM in $(seq 1 $NUMTESTS); do
  rm -f WF-outfile$NUM.txt
  timeout 10 WordFrequency WF-infile$NUM.txt WF-outfile$NUM.txt
  diff -bBwu WF-outfile$NUM.txt Model-WF-outfile$NUM.txt &> WF-diff$NUM.txt >> WF-diff$NUM.txt
  echo "WordFrequency Test $NUM:"
  echo "=========="
  cat WF-diff$NUM.txt
  echo "=========="
  if [ -e WF-diff$NUM.txt ] && [[ ! -s WF-diff$NUM.txt ]]; then
    let lextestspassed+=1
  fi
done

let lextestpoints=${PNTSPERTEST}*lextestspassed

echo "Passed $lextestspassed / $NUMTESTS WordFrequency tests"
echo "This gives a total of $lextestpoints / $MAXPTS points"
echo ""
echo ""

rm -f *.o WordFrequency WF-diff*
