#!/bin/bash
rm -f test_async_result.txt
rm -f test_async_id.txt

if [ ! -f ./test.out ]; then
    echo "Executable test.out not found! Compile it ..."
    make
fi

./test.out
