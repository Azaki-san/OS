#!/bin/bash

echo "File types in /dev:"
file /dev/*

echo "File types in /etc:"
file /etc/*
dir_count=$(ls -l /etc | grep ^d | wc -l)
echo "Number of directories in /etc: $dir_count"

echo -e '#include <stdio.h>\nint main() { printf("Привет, мир!"); return 0; }' > ex1.c

echo "File type of ex1.c before compilation:"
file ex1.c

gcc -o ex1 ex1.c

echo "File type of ex1 after compilation:"
file ex1

