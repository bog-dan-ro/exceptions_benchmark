#!/bin/sh

uname -a
for compiler in g++ clang++; do
    $compiler -v
    for flags in -O1 -O2 -O3 -Ofast -Os; do
        echo "\n\n$compiler $flags -std=c++11 main.cpp -o test && ./test"
        $compiler $flags -std=c++11 main.cpp -o test && ./test
    done
done

rm test
