#!/usr/bin/bash

SRCDIR=https://raw.githubusercontent.com/houi-lin/cse101-pt.w21/master/pa7
TIME=8

if [ ! -e backup ]; then
   echo "WARNING: a backup has been created for you in the \"backup\" folder"
   mkdir backup
fi


cp *.c *.h Makefile backup   # copy all files of importance into backup

curl $SRCDIR/ModelBigIntegerTest.c > ModelBigIntegerTest.c

echo ""
echo ""

echo "Press Enter To Continue with ModelBigIntegerTest Results"
read verbose

echo ""
echo ""

gcc -c -std=c11 -Wall -g ModelBigIntegerTest.c BigInteger.c List.c -lm
gcc -o ModelBigIntegerTest ModelBigIntegerTest.o BigInteger.o List.o -lm

timeout $TIME valgrind --leak-check=full -v ./ModelBigIntegerTest -v > BITest-out.txt

cat BITest-out.txt

rm -f *.o BITest-out.txt
