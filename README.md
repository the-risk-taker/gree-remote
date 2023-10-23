# gree-remote

Original project repo can be found here: <https://github.com/the-risk-taker/gree-remote>.

> In this fork I only added some CMake to show how to integrate Clang-Format, Clang-Tidy and Cppcheck - for the purpose of my talk.

## Setup

> NOTE: Works only on Linux.

After clone, fetch submodules `git submodule update --init --recursive`. Then setup CMake `cmake -S . -B build -DCMAKE_PREFIX_PATH=<qt_version_toolchain_path> -G Ninja`, then compile `cryptopp` using `ninja -C build build-cryptopp`, then build project `ninja -C build` and finally try Clang-Format, Clang-Tidy and Cppcheck targets with Ninja.
