#!/bin/bash
gcc ex3.c -o ex3
./ex3 3 &
for i in {1..3}
do
    sleep 5
    pstree -p -s $$
done
sleep 5

./ex3 5 &
for i in {1..5}
do
    sleep 5
    pstree -p -s $$
done
