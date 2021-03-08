#!/usr/bin/bash

SRCDIR=https://raw.githubusercontent.com/houi-lin/cse101-pt.w21/master/pa7

if [ ! -e backup ]; then
  mkdir backup
fi

cp *.c *.h Makefile backup   # copy all files of importance into backup


echo ""
echo ""

rm -f *.o Arithmetic

make

if [ ! -e Arithmetic ] || [ ! -x Arithmetic ]; then # exist and executable
  echo ""
  echo "Makefile probably doesn't correctly create Executable called \"Arithmetic\"!!!"
  echo ""
else
  echo ""
  echo "Makefile probably correctly creates Executable!"
  echo ""
fi

echo ""
echo ""

make clean

if [ -e Arithmetic ] || [ -e *.o ]; then
  echo "WARNING: Makefile didn't successfully clean all files"
fi

rm -f *.o Arithmetic garbage
