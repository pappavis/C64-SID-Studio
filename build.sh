#!/usr/bin/env bash
set -e
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
ctest --test-dir build --output-on-failure
auval -v aumu Si64 Vl71
