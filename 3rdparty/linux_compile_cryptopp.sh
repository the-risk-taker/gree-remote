#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/cryptopp"
make -j$(nproc) -C "$DIR" CXX="g++"
