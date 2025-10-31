#!/bin/bash

if [ "$(uname -s)" == "Darwin" ]; then
    g++ -std=c++17 -o main main.cpp \
        -isystem /opt/homebrew/Cellar/googletest/1.17.0/include \
        -L/opt/homebrew/Cellar/googletest/1.17.0/lib \
        -lgtest -lgtest_main -pthread
else
    g++ -o main main.cpp -lgtest -lgtest_main -pthread
fi
./main