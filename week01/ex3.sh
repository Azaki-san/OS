#!/bin/sh
date
sleep 3
mkdir ~/rootF
date
sleep 3
mkdir ~/homeF
date
sleep 3
cd ~/rootF
touch root.txt
date
sleep 3
cd ~/homeF
touch home.txt
date
sleep 3
cd ~/rootF
ls -ltr / > ~/rootF/root.txt
cat ~/rootF/root.txt
ls
cd ~/homeF
ls -ltr ~ > ~/homeF/home.txt
cat ~/homeF/home.txt
ls
