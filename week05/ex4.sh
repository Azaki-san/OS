#!/bin/bash

gcc ex4.c -o ex4 -lpthread

output_file="ex4_res.txt"

> "$output_file"

n=10000000
m_values=(1 2 4 10 100)

for m in "${m_values[@]}"; do
    echo "Running with n = $n and m = $m"
    (time ./ex4 $n $m) 2>&1 | tee -a $output_file
done

rm ex4

echo "Execution times for different values of m:"
cat $output_file

