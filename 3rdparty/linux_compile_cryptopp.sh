#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/cryptopp"
CXX="g++" make -j$(nproc) -C "$DIR" libcryptopp.a
