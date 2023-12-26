#!/bin/bash
rm -rf build && mkdir build && \
  cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_CXX_COMPILER=clang++-15 \
  -DCMAKE_C_COMPILER=clang-15 \
  -S. \
  -B./build \
  -G Ninja
