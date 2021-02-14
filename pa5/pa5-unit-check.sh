#!/usr/bin/bash

SRCDIR=https://raw.githubusercontent.com/houi-lin/cse101-pt.w21/master/pa5

if [ ! -e backup ]; then
   echo "WARNING: a backup has been created for you in the \"backup\" folder"
   mkdir backup
fi


cp *.c *.h Makefile backup   # copy all files of importance into backup

curl $SRCDIR/ModelDictionaryTest.c > ModelDictionaryTest.c

echo ""
echo ""

echo "Press Enter To Continue with Dictionary Unit Test"
read verbose

echo ""
echo ""

gcc -c -std=c11 -Wall -g ModelDictionaryTest.c Dictionary.c
gcc -o ModelDictionaryTest ModelDictionaryTest.o Dictionary.o

timeout 5 valgrind --leak-check=full -v ./ModelDictionaryTest -v > DictionaryTest-out.txt 

cat DictionaryTest-out.txt 

rm -f *.o ModelDictionaryTest DictionaryTest-out.txt 

