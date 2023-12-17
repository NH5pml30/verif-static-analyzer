#!/bin/bash
rm -rf build && mkdir build && \
  cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -S. \
  -B./build \
  -G Ninja
