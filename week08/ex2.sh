#!/bin/bash

mkdir -p /tmp/ex2

gcc mmu.c -o mmu
gcc pager.c -o pager

NUM_PAGES=4
NUM_FRAMES=2

./pager $NUM_PAGES $NUM_FRAMES &

PAGER_PID=$!

gnome-terminal --tab --title="MMU" -- bash -c "./mmu $NUM_PAGES 'R0 R1 W1 R0 R2 W2 R0 R3 W2' $PAGER_PID ; exec bash"

MMU_STATUS=$?
if [ $MMU_STATUS -ne 0 ]; then
    echo "MMU process exited with an error (status code: $MMU_STATUS)." >&2
    exit 1
fi

wait $PAGER_PID

rm mmu pager