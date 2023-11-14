#!/bin/bash

rm -rf ./tmp
ln -s ./tmp tmp1
ls -li
mkdir ./tmp
ls -li
./gen.sh > ./tmp/ex1.txt
ls -li ./tmp
ln -s $(realpath ./tmp) ./tmp1/tmp2
./gen.sh > ./tmp1/tmp2/ex1.txt

ls -li ./tmp
ls -li ./tmp1
ls -li ./tmp1/tmp2
rm -rf ./tmp
ls -li ./tmp1
ls -li ./tmp1/tmp2
rm ./tmp1

