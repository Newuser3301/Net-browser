#!/usr/bin/env bash
set -euo pipefail
echo "Building Net-browser core (CMake)..."
mkdir -p build
cd build
cmake .. -G "Unix Makefiles"
cmake --build . -j
echo "Done ✅"