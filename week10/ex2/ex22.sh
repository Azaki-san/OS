#!/bin/bash

inode_ex1=$(stat -c "%i" ex1)
echo "Inode of ex1: $inode_ex1"

block_count_ex1=$(stat -c "%b" ex1)
block_size_ex1=$(stat -c "%B" ex1)
echo "Number of blocks for ex1: $block_count_ex1"
echo "Size of each block for ex1: $block_size_ex1 bytes"

size_ex1=$(stat -c "%s" ex1)
permissions_ex1=$(stat -c "%A" ex1)
echo "Total size of ex1: $size_ex1 bytes"
echo "Permissions of ex1: $permissions_ex1"

cp ex1 ex2

link_count_ex2=$(stat -c "%h" ex2)
echo "Number of links for ex2: $link_count_ex2"

inode_ex2=$(stat -c "%i" ex2)

# Compare inode numbers of ex1 and ex2
if [ "$inode_ex1" -eq "$inode_ex2" ]; then
    echo "ex1 and ex2 have the same i-node numbers."
else
    echo "ex1 and ex2 do not have the same i-node numbers."
fi

echo "Files with 3 links in /etc:"
stat -c "%h - %n" /etc/* | grep '^3'

# Explanation of the number 3 links
echo "The number 3 represents the number of hard links to the file. Hard links are additional directory entries pointing to the same inode."

