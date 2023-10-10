#!/bin/bash

gcc ex3.c -o ex3 -lpthread

output_file="ex3_res.txt"

> "$output_file"

n=10000000
m_values=(1 2 4 10 100)

for m in "${m_values[@]}"; do
    echo "Running with n = $n and m = $m"
    (time ./ex3 $n $m) 2>&1 | tee -a $output_file
done

rm ex3

echo "Execution times for different values of m:"
cat $output_file

