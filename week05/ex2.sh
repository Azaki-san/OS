#!/bin/bash

gcc ex2.c -o ex2 -pthread

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <n>"
    exit 1
fi

./ex2 "$1"

rm ex2

