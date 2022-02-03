#!/bin/bash

make

echo "Original test program:"
./test

echo "Now with my_fork.so loaded:"
LD_PRELOAD=$PWD/my_fork.so ./test

make clean