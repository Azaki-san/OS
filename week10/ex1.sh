#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Error"
    exit 1
fi
filename="$1"
gcc monitor.c -o monitor
gcc ex1.c -o ex1
if [ $? -eq 0 ]; then
gnome-terminal --tab --title="MONITOR" -- bash -c "./monitor $filename; exec bash"
gnome-terminal --tab --title="Ex1" -- bash -c "./ex1 $filename; exec bash"
else
  echo "Compilation failed."
fi

