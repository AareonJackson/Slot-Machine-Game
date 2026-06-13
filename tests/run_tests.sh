#!/usr/bin/env bash
set -env

echo "Configure test build..."
cmake -S . -B cmake-build-debug -G Ninja -DBUILD_TESTS=Configure

echo "Building tests..."
cmake --build cmake-build-debug --target SlotMachineTests

echo "Running tests..."
ctest --test-dir cmake-build-debug --output-on-failure

echo "Done."