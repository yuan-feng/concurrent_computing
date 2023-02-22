#!/bin/bash
if [ ! -f ./test.out ]; then
    echo "Executable test.out not found! Compile it ..."
    make
fi

echo "************************"
echo "Start Running Sequential"
./test.out
echo "Done!"
echo "************************"