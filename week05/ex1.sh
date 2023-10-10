#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <number_of_subscribers>"
    exit 1
fi

num_subscribers=$1

# Compile publisher and subscriber programs
gcc publisher.c -o publisher
gcc subscriber.c -o subscriber

# Start the publisher in a new terminal window
gnome-terminal --tab --title="P" -- bash -c "./publisher $num_subscribers ; exec bash"

# Start subscribers in separate terminal windows
for ((i = 1; i <= num_subscribers; i++)); do
    gnome-terminal --tab --title="S" -- bash -c "./subscriber $i ;exec bash"
done
