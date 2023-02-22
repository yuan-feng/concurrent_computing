#!/bin/bash
if [ ! -f ./test.out ]; then
    echo "Executable test.out not found! Compile it ..."
    make
fi

echo "************************"
echo "Start Running Parallel"
mpirun -np 4 ./test.out
echo "Done!"
echo "************************"