#!/usr/bin/env bash
set -e

echo "Configure test build..."
cmake -S . -B cmake-build-debug -G Ninja -DBUILD_TESTS=ON

echo "Building tests..."
cmake --build cmake-build-debug --target SlotMachineTests

echo "Running tests..."
ctest --test-dir cmake-build-debug --output-on-failure

echo "Done."