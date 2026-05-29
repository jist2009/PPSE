#!/bin/bash

./simulator -m 0 -M 12 -s 1 -e 100 -K 32 -N 32 -D "rep-soft8-neon"  -f 6 -S 8 -o 5
./simulator -m 0 -M 12 -s 1 -e 100 -K 32 -N 64 -D "rep-soft8-neon"  -f 1 -S 6 -o 4

./simulator -m 0 -M 15 -s 1 -e 100 -K 32 -N 128 -D "rep-hard8-neon" -f 2 -S 4 -o 1
./simulator -m 0 -M 12 -s 1 -e 100 -K 32 -N 128 -D "rep-soft8-neon" -f 2 -S 7 -o 2

./simulator -m 0 -M 12 -s 1 -e 100 -K 32 -N 96 -D "rep-soft8-neon"  -f 0 -S 8 -o 3
